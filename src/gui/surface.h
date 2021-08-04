#pragma once
#include "../gfx/animated_sprite/animated_sprite.h"

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include <variant>


namespace gui
{
    class Surface : public sf::Drawable
    {
        using surface_type = std::variant<sf::RectangleShape, sf::Sprite, Animated_sprite>;
        mutable surface_type surf;

        void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

    public:
        Surface() {}
        Surface(const surface_type& s) : surf(s) {}

        void setSize(const sf::Vector2f& size);
        void setPosition(const sf::Vector2f& position);
    };
}