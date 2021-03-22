#include "map_gen.h"

#include <algorithm>
#include <fstream>
#include <time.h>
#include <stdlib.h>

#include <iostream>

int rand(int min, int max)
{
    auto initSeed = []()
    {
        int s = time(0);
        std::ofstream("seed.txt") << s;
        return s;
    };
    static int seed = initSeed();
    srand(seed++);

    return min + rand() % abs(max+1 - min);
}


void map::setSize(const vec2i size)
{
    tiles.resize(size.x);
    for (auto& i : tiles)
    {
        i.resize(size.y);
    }
}
vec2i map::getSize() { return tiles.size() > 0 ? vec2i{(int)tiles.size(), (int)tiles[0].size()} : vec2i{0, 0}; }

TILE_TYPE& map::at(const vec2i pos) { return tiles[pos.x][pos.y]; }

void map::fill(const TILE_TYPE ttype)
{
    for (auto& i : tiles)
    {
        for (auto& j : i)
        {
            j = ttype;
        }
    }
}

void map::printToFile(const std::string& fname)
{
    std::ofstream out(fname);
    for (int y = 0; y < getSize().y; y++)
    {
        for (int x = 0; x < getSize().x; x++)
        {
            switch (at({ x,y }))
            {
            case TILE_TYPE::WALL:
                out << '.';
                break;
            case TILE_TYPE::ROOM:
                out << 'R';
                break;
            case TILE_TYPE::HALLWAY:
                out << 'H';
            }
        }
        out << '\n';
    }
    out.close();
}

bool map::isPositionValid(const vec2i pos)
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < getSize().x && pos.y < getSize().y;
}

void map::setTiles(const rect_i r, const TILE_TYPE tile)
{
    for (int x = r.tl.x; x<= r.br.x;x++)
    {
        for (int y = r.tl.y;y<= r.br.y;y++)
        {
            if (isPositionValid({ x,y }))
            {
                at({ x,y }) = tile;
            }
        }
    }
}
void map::generateHallway(const vec2i start_p, const gen_params params)
{
    auto oppositeDir = [](const DIRECTION dir)
    {
        switch (dir)
        {
        case DIRECTION::LEFT:
            return DIRECTION::RIGHT;
        case DIRECTION::UP:
            return DIRECTION::DOWN;
        case DIRECTION::RIGHT:
            return DIRECTION::LEFT;
        case DIRECTION::DOWN:
            return DIRECTION::UP;
        }
    };
    auto randomDir = [&](const DIRECTION dir, const DIRECTION forbidden_dir, const vec2i pos)
    {
        std::vector<DIRECTION> dirs = { DIRECTION::LEFT, DIRECTION::UP, DIRECTION::RIGHT, DIRECTION::DOWN };
        auto remDir = [&](const DIRECTION dir)
        {
            if (dirs.size() > 1)
            {
                for (int i = 0; i < dirs.size(); i++)
                {
                    if (dirs[i] == dir) { dirs.erase(dirs.begin() + i); break; }
                }
            }
        };
        remDir(oppositeDir(dir));

        if (   !(pos.x <= 0               && pos.y <= 0            )
            && !(pos.x <= 0               && pos.y >= getSize().y-1)
            && !(pos.x >= getSize().x - 1 && pos.y <= 0            )
            && !(pos.x >= getSize().x - 1 && pos.y >= getSize().y-1))//ignore forbidden_dir in corners
        {
            remDir(forbidden_dir);
        }
        if (pos.x >= getSize().x - 1) { remDir(DIRECTION::RIGHT); }
        if (pos.x <= 0) { remDir(DIRECTION::LEFT); }
        if (pos.y >= getSize().y - 1) { remDir(DIRECTION::DOWN); }
        if (pos.y <= 0) { remDir(DIRECTION::UP); }

        return dirs[rand(0, dirs.size() - 1)];
    };
    auto adjacentHallwayValidC = [&](const vec2i pos, const DIRECTION dir)
    {
        switch (dir)
        {
        case DIRECTION::LEFT:
        case DIRECTION::RIGHT:
            return (isPositionValid(pos + vec2i{ 0,1 }) ? at(pos + vec2i{ 0,1 }) != TILE_TYPE::HALLWAY : true)
                && (isPositionValid(pos + vec2i{ 0,-1 }) ? at(pos + vec2i{ 0,-1 }) != TILE_TYPE::HALLWAY : true);
        case DIRECTION::UP:
        case DIRECTION::DOWN:
            return (isPositionValid(pos + vec2i{ 1,0 }) ? at(pos + vec2i{ 1,0 }) != TILE_TYPE::HALLWAY : true)
                && (isPositionValid(pos + vec2i{ -1,0 }) ? at(pos + vec2i{ -1,0 }) != TILE_TYPE::HALLWAY : true);
        }
    };
    auto initDir = [&]()
    {
        if (isPositionValid(start_p + vec2i{ -1,  0 }) && at(start_p + vec2i{ -1,  0 }) != TILE_TYPE::WALL) { return DIRECTION::RIGHT; }
        if (isPositionValid(start_p + vec2i{  1,  0 }) && at(start_p + vec2i{  1,  0 }) != TILE_TYPE::WALL) { return DIRECTION::LEFT; }
        if (isPositionValid(start_p + vec2i{  0, -1 }) && at(start_p + vec2i{  0, -1 }) != TILE_TYPE::WALL) { return DIRECTION::DOWN; }
        if (isPositionValid(start_p + vec2i{  0,  1 }) && at(start_p + vec2i{  0,  1 }) != TILE_TYPE::WALL) { return DIRECTION::UP; }
        return DIRECTION::UP;
    };

    if (adjacentTileCount(start_p, false, TILE_TYPE::HALLWAY) > 0)
    {
        return;
    }
    DIRECTION dir = initDir();
    const DIRECTION forbidden_dir = oppositeDir(dir);

    vec2i c_pos = start_p;
    const unsigned int m_segment_count = 
        params.max_hallway_segment_count >= params.min_hallway_segment_count
        ? rand(params.min_hallway_segment_count, params.max_hallway_segment_count)-1
        : 0;

    unsigned int total_len = 0;
    for (int seg = m_segment_count; seg >= 0; seg--)
    {
        const unsigned int m_seg_len = rand(params.min_hallway_segment_length, params.max_hallway_segment_length);

        for (unsigned int len = 0; len < m_seg_len; len++, total_len++)
        {
            if (   !(c_pos.x <= 0             && dir == DIRECTION::LEFT )
                && !(c_pos.y <= 0             && dir == DIRECTION::UP   )
                && !(c_pos.x >= getSize().x-1 && dir == DIRECTION::RIGHT)
                && !(c_pos.y >= getSize().y-1 && dir == DIRECTION::DOWN ))
            {
                if (at(c_pos) == TILE_TYPE::WALL)
                {
                    at(c_pos) = TILE_TYPE::HALLWAY;
                    if (adjacentTileCount(c_pos, false, TILE_TYPE::HALLWAY) > 1) { return; }
                    if (total_len >= 1 && adjacentTileCount(c_pos, false, TILE_TYPE::ROOM) > 0) { return; }
                    if (len >= 1 && !adjacentHallwayValidC(c_pos, dir)) { return; }

                    switch (dir)
                    {
                    case DIRECTION::LEFT:
                        c_pos.x--;
                        break;
                    case DIRECTION::UP:
                        c_pos.y--;
                        break;
                    case DIRECTION::RIGHT:
                        c_pos.x++;
                        break;
                    case DIRECTION::DOWN:
                        c_pos.y++;
                    }
                }
                else return;
            }
            else break;
        }

        if (seg == 0)
        {
            if (isPositionValid(c_pos) && at(c_pos) == TILE_TYPE::WALL)
            {
                if (!generateRoom(c_pos, params))
                {
                    seg++;
                }
            }
        }
        dir = randomDir(dir, forbidden_dir, c_pos);
    }
}

bool map::generateRoom(const vec2i start_p, const gen_params params)
{
    if(adjacentTileCount(start_p, true, TILE_TYPE::ROOM) > 0)
    {
        return false;
    }

    rect_i r = { start_p,start_p };
    const vec2i max_size = 
    { 
        rand(params.min_room_size.x, params.max_room_size.x),
        rand(params.min_room_size.y, params.max_room_size.y) 
    };

    bool left_expansion_possible   = true;
    bool top_expansion_possible    = true;
    bool right_expansion_possible  = true;
    bool bottom_expansion_possible = true;

    while (r.size().x < max_size.x || r.size().y < max_size.y)
    {
        if(left_expansion_possible)
        {
            const rect_i scan_area = { r.tl + vec2i{-2,-1}, r.tl + vec2i{-1,r.size().y} };
            if (r.tl.x <=0 || tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area))
            {
                left_expansion_possible = false;
            }
        }
        if (left_expansion_possible && r.size().x < max_size.x) { r.tl.x--; }

        if (top_expansion_possible)
        {
            const rect_i scan_area = { r.tl + vec2i{-1,-2}, r.tl + vec2i{r.size().x,-1} };
            if (r.tl.y<=0 || tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area))
            {
                top_expansion_possible = false;
            }
        }
        if (top_expansion_possible && r.size().y < max_size.y) { r.tl.y--; }

        if (right_expansion_possible)
        {
            const rect_i scan_area = { r.br + vec2i{1,-r.size().y}, r.br + vec2i{2,1} };
            if (r.br.x >= getSize().x-1 || tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area))
            {
                right_expansion_possible = false;
            }
        }
        if (right_expansion_possible && r.size().x < max_size.x) { r.br.x++; }

        if (bottom_expansion_possible)
        {
            const rect_i scan_area = { r.br + vec2i{-r.size().x, 1}, r.br + vec2i{1,2} };
            if (r.br.y >= getSize().y-1 || tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area))
            {
                bottom_expansion_possible = false;
            }
        }
        if (bottom_expansion_possible && r.size().y < max_size.y) { r.br.y++; }
            
        if (   ((!left_expansion_possible && !right_expansion_possible)  || r.size().x >= max_size.x)
            && ((!top_expansion_possible  && !bottom_expansion_possible) || r.size().y >= max_size.y))
        {
            break; 
        }
    }

    if (r.size().x >= params.min_room_size.x && r.size().y >= params.min_room_size.y)
    {
        setTiles(r, TILE_TYPE::ROOM);

        const unsigned int max_door_c = rand(params.min_door_count, params.max_door_count);      
        std::vector<vec2i> possible_door_pos;

        for (int x = 0; x < r.size().x; x++)
        {
           possible_door_pos.push_back(r.tl + vec2i{ x,-1 });
           possible_door_pos.push_back(r.tl + vec2i{ x,r.size().y });
        }
        for (int y = 0; y < r.size().y; y++)
        {
            possible_door_pos.push_back(r.tl + vec2i{ -1,y });
            possible_door_pos.push_back(r.tl + vec2i{ r.size().x,y });
        }
        for (int i = possible_door_pos.size()-1; i > 0; i--)
        {
            if (isPositionValid(possible_door_pos[i]))
            {
                if (at(possible_door_pos[i]) == TILE_TYPE::HALLWAY)
                {
                    possible_door_pos.erase(possible_door_pos.begin() + i);
                }
            }
            else { possible_door_pos.erase(possible_door_pos.begin() + i); }
        }

        std::vector<vec2i> door_pos;
        for (int i = 0; i < max_door_c && possible_door_pos.size()>0; i++)
        {
            int r_pos = rand(0, possible_door_pos.size() - 1);
            door_pos.push_back(possible_door_pos[r_pos]);
            possible_door_pos.erase(possible_door_pos.begin() + r_pos);
        }

        for (int i = 0; i < door_pos.size(); i++)
        {
            generateHallway(door_pos[i], params);
        }
    }
    else return false;

    return true;
}

void map::generate(const gen_params params)
{
    fill(TILE_TYPE::WALL);

    generateRoom(getSize()/2, params);
}

unsigned int map::tileCount(const rect_i r)
{
    unsigned int c = 0;
    for (int x = r.tl.x; x <= r.br.x; x++)
    {
        for (int y = r.tl.y; y <= r.br.y; y++)
        {
            if (isPositionValid({ x,y })) { c++; }
        }
    }
    return c;
}

unsigned int map::tileCount(const rect_i r, const TILE_TYPE ttype)
{
    unsigned int c = 0;
    for (int x = r.tl.x; x <= r.br.x; x++)
    {
        for (int y = r.tl.y; y <= r.br.y; y++)
        {
            if (isPositionValid({ x,y }) && at({x,y}) == ttype) { c++; }
        }
    }
    return c;
}

unsigned int map::adjacentTileCount(const vec2i pos, const bool diag_check)
{
    unsigned int c = 0;
    switch (diag_check)
    {
    case true:
    {
        for (int x = pos.x - 1; x <= pos.x + 1; x++)
        {
            for (int y = pos.y - 1; y <= pos.y + 1; y++)
            {
                if (vec2i{ x,y } != pos && isPositionValid({ x,y }))
                {
                    c++;
                }
            }
        }
        break;
    }
    case false:
        c+= isPositionValid(pos - vec2i{ 1,0 })
          + isPositionValid(pos + vec2i{ 1,0 })
          + isPositionValid(pos - vec2i{ 0,1 })
          + isPositionValid(pos + vec2i{ 0,1 });
    }
    return c;
}

unsigned int map::adjacentTileCount(const vec2i pos, const bool diag_check, const TILE_TYPE ttype)
{
    unsigned int c = 0;
    switch (diag_check)
    {
    case true:
    {
        for (int x = pos.x - 1; x <= pos.x + 1; x++)
        {
            for (int y = pos.y - 1; y <= pos.y + 1; y++)
            {
                if (vec2i{ x,y } != pos && isPositionValid({ x,y }) && at({x,y}) == ttype)
                {
                    c++;
                }
            }
        }
        break;
    }
    case false:
        c += (isPositionValid(pos - vec2i{ 1,0 }) ? at(pos - vec2i{ 1,0 }) == ttype : 0)
           + (isPositionValid(pos + vec2i{ 1,0 }) ? at(pos + vec2i{ 1,0 }) == ttype : 0)
           + (isPositionValid(pos - vec2i{ 0,1 }) ? at(pos - vec2i{ 0,1 }) == ttype : 0)
           + (isPositionValid(pos + vec2i{ 0,1 }) ? at(pos + vec2i{ 0,1 }) == ttype : 0);
    }
    return c;
}
