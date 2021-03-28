#include "level_structure.h"

#include <fstream>

void level_structure::setSize(const vec2i size)
{
    tiles.resize(size.x);
    for (auto& i : tiles)
    {
        i.resize(size.y);
    }
}
vec2i level_structure::getSize() 
{
    return tiles.size() > 0 ? vec2i{ (int)tiles.size(), (int)tiles[0].size() } : vec2i{ 0, 0 }; 
}

TILE_TYPE& level_structure::at(const vec2i pos) 
{
    return tiles[pos.x][pos.y]; 
}

bool level_structure::isPositionValid(const vec2i pos)
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < getSize().x&& pos.y < getSize().y;
}

void level_structure::printToFile(const std::string& fname)
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