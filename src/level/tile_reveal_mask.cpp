#include "tile_reveal_mask.h"

void Tile_reveal_mask::setSize(const sf::Vector2i& size)
{
    mask.resize(size.x);
    for (auto& i : mask)
    {
        i.resize(size.y);
    }
}

sf::Vector2i Tile_reveal_mask::getSize() const
{
    return mask.size() > 0 ? sf::Vector2i{ (int)mask.size(), (int)mask[0].size() } : sf::Vector2i{ 0, 0 };
}

Tile_visibility_info& Tile_reveal_mask::at(const sf::Vector2i& pos)
{
    return mask[pos.x][pos.y];
}

const Tile_visibility_info& Tile_reveal_mask::at(const sf::Vector2i& pos) const
{
    return mask[pos.x][pos.y];
}

void Tile_reveal_mask::create(const sf::Vector2i& size)
{
    setSize(size);
    for (int x = 0; x < getSize().x; x++)
    {
        for (int y = 0; y < getSize().y; y++)
        {
            at({ x,y }) = Tile_visibility_info{};
        }
    }
}

void Tile_reveal_mask::reveal(const std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>& tiles)
{
    for (const auto& tile : tiles)
    {
        const auto& [position, tvi] = tile;

        if (position.x >= 0 && position.x <= getSize().x
            && position.y >= 0 && position.y <= getSize().y)
        {
            at(position).combine(tvi);
        }
    }
}