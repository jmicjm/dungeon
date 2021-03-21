#include "map_gen.h"

#include <fstream>
#include <time.h>
#include <stdlib.h>




int rand(int min, int max)
{
    static int seed = time(0);
    srand(seed++);

    return min + rand() % abs(max - min);
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

bool map::isTileAdjacentTo(const vec2i pos, TILE_TYPE t, bool diag_check)
{
    switch (diag_check)
    {
    case true:
    {
        for (int x = pos.x-1; x <= pos.x + 1; x++)
        {
            for (int y = pos.y-1; y <= pos.y + 1; y++)
            {
                if (vec2i{ x,y } != pos && isPositionValid({x,y}))
                {
                    if (at({x,y}) == t) { return true; }
                }
            }
        }
        return false;
    }
    case false:
        return (isPositionValid(pos - vec2i{ 1,0 }) ? at(pos - vec2i{ 1,0 }) == t : false)
            || (isPositionValid(pos + vec2i{ 1,0 }) ? at(pos + vec2i{ 1,0 }) == t : false)
            || (isPositionValid(pos - vec2i{ 0,1 }) ? at(pos - vec2i{ 0,1 }) == t : false)
            || (isPositionValid(pos + vec2i{ 0,1 }) ? at(pos + vec2i{ 0,1 }) == t : false);
    }
}
bool map::isTileAdjacentOnlyTo(const vec2i pos, TILE_TYPE t, bool diag_check)
{
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
                    if (at({ x,y }) != t) { return false; }
                }
            }
        }
        return true;
    }
    case false:
        return (isPositionValid(pos - vec2i{ 1,0 }) ? at(pos - vec2i{ 1,0 }) == t : true)
            && (isPositionValid(pos + vec2i{ 1,0 }) ? at(pos + vec2i{ 1,0 }) == t : true)
            && (isPositionValid(pos - vec2i{ 0,1 }) ? at(pos - vec2i{ 0,1 }) == t : true)
            && (isPositionValid(pos + vec2i{ 0,1 }) ? at(pos + vec2i{ 0,1 }) == t : true);
    }
}

void map::generateHallway(const vec2i start_p, const gen_params params, const DIRECTION direction)
{
    vec2i c_pos = start_p;

    unsigned int m_len = rand(params.min_hallway_length, params.max_hallway_length);

    for (unsigned int len = 0; len < m_len; len++)
    {
        if (isPositionValid(c_pos) && at(c_pos) == TILE_TYPE::WALL)
        {
            at(c_pos) = TILE_TYPE::HALLWAY;
            switch (direction)
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
        else break;
            
    }

    if (isPositionValid(c_pos) && at(c_pos) == TILE_TYPE::WALL)
    {
        generateRoom(c_pos, params);
    }
}

void map::generateRoom(const vec2i start_p, const gen_params params)
{
    rect_i r = { start_p,start_p };

    if(isTileAdjacentTo(r.tl, TILE_TYPE::ROOM, true))
    {
        return;
    }

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
            if (tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area))
            {
                left_expansion_possible = false;
            }
        }
        if (left_expansion_possible && r.size().x < max_size.x) { r.tl.x--; }

        if (top_expansion_possible)
        {
            const rect_i scan_area = { r.tl + vec2i{-1,-2}, r.tl + vec2i{r.size().x,-1} };
            if (tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area))
            {
                top_expansion_possible = false;
            }
        }
        if (top_expansion_possible && r.size().y < max_size.y) { r.tl.y--; }

        if (right_expansion_possible)
        {
            const rect_i scan_area = { r.br + vec2i{1,-r.size().y}, r.br + vec2i{2,1} };
            if (tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area))
            {
                right_expansion_possible = false;
            }
        }
        if (right_expansion_possible && r.size().x < max_size.x) { r.br.x++; }

        if (bottom_expansion_possible)
        {
            const rect_i scan_area = { r.br + vec2i{-r.size().x, 1}, r.br + vec2i{1,2} };
            if (tileCount(scan_area, TILE_TYPE::WALL) < tileCount(scan_area))
            {
                bottom_expansion_possible = false;
            }
        }
        if (bottom_expansion_possible && r.size().y < max_size.y) { r.br.y++; }
            
        if (   ((!left_expansion_possible && !right_expansion_possible) || r.size().x >= max_size.x)
            && ((!top_expansion_possible && !bottom_expansion_possible) || r.size().y >= max_size.y))
        {
            break; 
        }
    }

    if (r.size().x >= params.min_room_size.x && r.size().y >= params.min_room_size.y)
    {
        setTiles(r,TILE_TYPE::ROOM);

        if (isPositionValid({ r.tl.x - 1, r.tl.y }))
        {
            generateHallway({ r.tl.x - 1, r.tl.y }, params, DIRECTION::LEFT);
        }
        if (isPositionValid({ r.tl.x, r.tl.y - 1 }))
        {
            generateHallway({ r.tl.x, r.tl.y - 1 }, params, DIRECTION::UP);
        }
        if (isPositionValid({ r.br.x + 1, r.br.y }))
        {
            generateHallway({ r.br.x + 1, r.br.y }, params, DIRECTION::RIGHT);
        }
        if (isPositionValid({ r.br.x, r.br.y + 1 }))
        {
            generateHallway({ r.br.x, r.br.y + 1 }, params, DIRECTION::DOWN);
        }
    }

}

void map::generate(const gen_params params)
{
    fill(TILE_TYPE::WALL);

    generateRoom({ rand(0,getSize().x), rand(0, getSize().y) }, params);
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
