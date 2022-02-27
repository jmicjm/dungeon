#pragma once
#include "portal.h"
#include "../gfx/primitive_sprite.h"

#include <memory>


class Entrance
{
    Portal portal;
    Primitive_sprite sprite;

public:
    Entrance(Portal portal, Primitive_sprite sprite);

    const Portal& getPortal() const;
    const Primitive_sprite& getSprite() const;
};