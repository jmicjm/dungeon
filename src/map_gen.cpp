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

void map::generateHallway(const vec2i start_p, const unsigned int max_length, vec2i max_room_size, int direction)
{
    vec2i c_pos = start_p;

    unsigned int m_len = rand(1, max_length);

    for (unsigned int len = 0; len < m_len; len++)
    {
        if (isPositionValid(c_pos) && at(c_pos) == TILE_TYPE::WALL)
        {
            at(c_pos) = TILE_TYPE::HALLWAY;
            switch (direction)
            {
            case 0:
                c_pos.x--;
                break;
            case 1:
                c_pos.y--;
                break;
            case 2:
                c_pos.x++;
                break;
            case 3:
                c_pos.y++;
            }
        }
        else break;
            
    }

    if (isPositionValid(c_pos) && at(c_pos) == TILE_TYPE::WALL)
    {
        generateRoom(c_pos, max_room_size, max_length);
    }
}

void map::generateRoom(const vec2i start_p, const vec2i max_room_size, unsigned int max_hallway_length)
{
    rect_i r = { start_p,start_p };

    if(isTileAdjacentTo(r.tl, TILE_TYPE::ROOM, true))
    {
        return;
    }

    vec2i max_size = { rand(2, max_room_size.x), rand(2, max_room_size.y) };

    bool left_expansion_possible = true;
    bool top_expansion_possible = true;
    bool right_expansion_possible = true;
    bool bottom_expansion_possible = true;

    while (r.size().x < max_size.x || r.size().y < max_size.y)
    {
        if(left_expansion_possible)
        {
            for (vec2i exp = r.tl - vec2i{1,0}; exp.y <= r.br.y; exp.y++)
            {
                if (!isTileAdjacentOnlyTo(exp, TILE_TYPE::WALL, false))
                {
                    left_expansion_possible = false;
                    break;
                }
            }
        }
        if (left_expansion_possible && r.size().x < max_size.x) { r.tl.x--; }

        if (top_expansion_possible)
        {
            for (vec2i exp = r.tl - vec2i{0,1}; exp.x <= r.br.x; exp.x++)
            {
                if (!isTileAdjacentOnlyTo(exp, TILE_TYPE::WALL, false))
                {
                    top_expansion_possible = false;
                    break;
                }
            }
        }
        if (top_expansion_possible && r.size().y < max_size.y) { r.tl.y--; }

        if (right_expansion_possible)
        {
            for (vec2i exp = r.br + vec2i{1,0}; exp.y >= r.tl.y; exp.y--)
            {
                    
                if (!isTileAdjacentOnlyTo(exp, TILE_TYPE::WALL, false))
                {
                    right_expansion_possible = false;
                    break;
                }
            }
        }
        if (right_expansion_possible && r.size().x < max_size.x) { r.br.x++; }

        if (bottom_expansion_possible)
        {
            for (vec2i exp = r.br + vec2i{0,1}; exp.x >= r.tl.x; exp.x--)
            {
                if (!isTileAdjacentOnlyTo(exp, TILE_TYPE::WALL, false))
                {
                    bottom_expansion_possible = false;
                    break;
                }
            }
        }
        if (bottom_expansion_possible && r.size().y < max_size.y) { r.br.y++; }
            
        if (   ((!left_expansion_possible && !right_expansion_possible) || r.size().x >= max_size.x)
            && ((!top_expansion_possible && !bottom_expansion_possible) || r.size().y >= max_size.y))
        {
            break; 
        }
    }

    if (r.size().x >= 2 && r.size().y >= 2)
    {
        setTiles(r,TILE_TYPE::ROOM);

        if (isPositionValid({ r.tl.x - 1, r.tl.y }))
        {
            generateHallway({ r.tl.x - 1, r.tl.y }, max_hallway_length, max_room_size, 0);
        }
        if (isPositionValid({ r.tl.x, r.tl.y - 1 }))
        {
            generateHallway({ r.tl.x, r.tl.y - 1 }, max_hallway_length, max_room_size, 1);
        }
        if (isPositionValid({ r.br.x + 1, r.br.y }))
        {
            generateHallway({ r.br.x + 1, r.br.y }, max_hallway_length, max_room_size, 2);
        }
        if (isPositionValid({ r.br.x, r.br.y + 1 }))
        {
            generateHallway({ r.br.x, r.br.y + 1 }, max_hallway_length, max_room_size, 3);
        }
    }

}

void map::generate(vec2i max_room_size, unsigned int max_hallway_length)
{
    fill(TILE_TYPE::WALL);

    generateRoom({ rand(0,getSize().x), rand(0, getSize().y) }, max_room_size, max_hallway_length);
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
