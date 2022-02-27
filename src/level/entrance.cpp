#include "entrance.h"
#include "../level/level.h"


Entrance::Entrance(Portal portal, Primitive_sprite sprite)
    : portal(std::move(portal)), sprite(sprite) {}

const Portal& Entrance::getPortal() const
{
    return portal;
}

const Primitive_sprite& Entrance::getSprite() const
{
    return sprite;
}
