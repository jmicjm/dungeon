#include "level_structure.h"

#include <fstream>

void Level_structure::setSize(const sf::Vector2i& size)
{
    tiles.resize(size.x);
    for (auto& i : tiles)
    {
        i.resize(size.y);
    }
}
sf::Vector2i Level_structure::getSize() const
{
    return tiles.size() > 0 ? sf::Vector2i{ (int)tiles.size(), (int)tiles[0].size() } : sf::Vector2i{ 0, 0 };
}

Tile& Level_structure::at(const sf::Vector2i& pos)
{
    return tiles[pos.x][pos.y]; 
}

const Tile& Level_structure::at(const sf::Vector2i& pos) const
{
    return tiles[pos.x][pos.y];
}

bool Level_structure::isPositionValid(const sf::Vector2i& pos) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < getSize().x&& pos.y < getSize().y;
}

size_t Level_structure::roomCount() const
{
    return room_rects.size();
}

Rect<sf::Vector2i> Level_structure::getRoomRect(size_t room_idx) const
{
    return room_rects[room_idx];
}

void Level_structure::printToFile(const std::string& fname) const
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