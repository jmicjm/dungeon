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

tile& level_structure::at(const vec2i pos) 
{
    return tiles[pos.x][pos.y]; 
}

bool level_structure::isPositionValid(const vec2i pos)
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < getSize().x&& pos.y < getSize().y;
}

size_t level_structure::roomCount()
{
    return room_rects.size();
}

rect_i level_structure::getRoomRect(size_t room_idx)
{
    return room_rects[room_idx];
}

void level_structure::printToFile(const std::string& fname)
{
    std::ofstream out(fname);
    for (int y = 0; y < getSize().y; y++)
    {
        for (int x = 0; x < getSize().x; x++)
        {
            switch (at({ x,y }).type)
            {
            case TILE_TYPE::WALL:
                out << '.';
                break;
            case TILE_TYPE::ROOM:
                out << 'R';
                break;
            case TILE_TYPE::HALLWAY:
                out << 'H';
                break;
            case TILE_TYPE::DOORWAY:
                out << 'D';
            }
        }
        out << '\n';
    }
    out.close();
}