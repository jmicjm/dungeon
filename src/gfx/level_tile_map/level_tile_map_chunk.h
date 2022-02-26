#pragma once
#include "../../level/level_structure.h"
#include "../combined_texture.h"

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


class Level_tile_map_chunk : public sf::Drawable
{
    Combined_texture* texture = nullptr;
    sf::VertexBuffer vertices;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

    void copyToBuffer(const sf::VertexArray& vertex_array);

public:
    Level_tile_map_chunk() = default;
    Level_tile_map_chunk(const Level_structure& ls, const sf::Vector2f& tile_size, const sf::IntRect& area, Combined_texture* texture);
    void setTexture(Combined_texture* tex);
};