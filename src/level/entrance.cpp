#include "entrance.h"
#include "../level/level.h"


Entrance::Entrance(std::weak_ptr<Level> level, sf::Vector2i destination_pos, Primitive_sprite sprite)
    : destination(std::move(level)), destination_pos(destination_pos), sprite(sprite) {}

const std::weak_ptr<Level>& Entrance::getDestination() const
{
    return destination;
}

const sf::Vector2i& Entrance::getDestinationPos() const
{
    return destination_pos;
}

const Primitive_sprite& Entrance::getSprite() const
{
    return sprite;
}
