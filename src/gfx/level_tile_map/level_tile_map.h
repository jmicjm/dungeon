#pragma once
#include "level_tile_map_chunk.h"
#include "../combined_texture.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>


class Level_tile_map : public sf::Drawable
{
    std::vector<std::vector<Level_tile_map_chunk>> chunks;
    sf::Vector2i chunk_size_px;

    Combined_texture texture;

    Level_tile_map_chunk& at(const sf::Vector2i& pos);
    const Level_tile_map_chunk& at(const sf::Vector2i& pos) const;

    void setSize(const sf::Vector2i& size);
    sf::Vector2i getSize() const;

    void setChunksTexture();

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    Level_tile_map() {}
    Level_tile_map(const Level_tile_map& other);
    Level_tile_map(Level_tile_map&& other);
    Level_tile_map& operator=(const Level_tile_map& other);
    Level_tile_map& operator=(Level_tile_map&& other);
    void populate(const Level_structure& ls, const sf::Vector2f& Tile_size, const sf::Vector2i& chunk_size);
};