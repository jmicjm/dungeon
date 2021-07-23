#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

struct Primitive_sprite
{
    const sf::Texture* texture = nullptr;

    constexpr static sf::PrimitiveType primitive_type = sf::Quads;
    sf::Vertex vertices[4];

    Primitive_sprite(const sf::Texture* texture, const sf::IntRect& rect);

    void move(const sf::Vector2f& offset);
    void moveTex(const sf::Vector2f& offset);
};