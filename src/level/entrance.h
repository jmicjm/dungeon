#pragma once
#include "../level/level.fwd.h"
#include "../gfx/primitive_sprite.h"

#include <memory>


class Entrance
{
    std::weak_ptr<Level> destination;
    sf::Vector2i destination_pos;
    Primitive_sprite sprite;

public:
    Entrance(std::weak_ptr<Level> destination, sf::Vector2i destination_pos, Primitive_sprite sprite);

    const std::weak_ptr<Level>& getDestination() const;
    const sf::Vector2i& getDestinationPos() const;
    const Primitive_sprite& getSprite() const;
};