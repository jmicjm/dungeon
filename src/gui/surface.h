#pragma once
#include "../gfx/animated_sprite/animated_sprite.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include <variant>


namespace gui
{
    class Surface
    {
        using surface_type = std::variant<sf::RectangleShape, sf::Sprite, Animated_sprite>;
        surface_type surf;

    public:
        Surface() {}
        Surface(const surface_type& s) : surf(s) {}

        void setSize(const sf::Vector2f& size);
        void setPosition(const sf::Vector2f& position);
        void draw(sf::RenderTarget& rt);
    };
}