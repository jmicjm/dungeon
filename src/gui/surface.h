#pragma once
#include "../gfx/animated_sprite/animated_sprite.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include <variant>


namespace gui
{
    using surface = std::variant<sf::RectangleShape, sf::Sprite, Animated_sprite>;

    void resizeSurface(surface& surf, const sf::Vector2f& size);
    void positionSurface(surface& surf, const sf::Vector2f& position);
    void drawSurface(surface& surf, sf::RenderTarget& rt);
}