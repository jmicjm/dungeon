#include "portal.h"
#include "../level/level.h"


bool usePortal(const Portal& portal, Position& position)
{
    if (auto destination = portal.destination_level.lock())
    {
        if (destination->isPassable(portal.destination_coords))
        {
            position.setPosition(portal.destination_coords, destination.get());
            return true;
        }
    }
    return false;
}