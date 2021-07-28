#pragma once
#include "tile_visibility_info.h"
#include <vector>

class Tile_reveal_mask
{
    std::vector<std::vector<Tile_visibility_info>> mask;

    void setSize(const sf::Vector2i& size);

public:
    sf::Vector2i getSize() const;
    Tile_visibility_info& at(const sf::Vector2i& pos);
    const Tile_visibility_info& at(const sf::Vector2i& pos) const;

    void create(const sf::Vector2i& size);
    void reveal(const std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>& tiles);
};