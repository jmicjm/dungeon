#include "usePortal.h"


bool usePortal(const Portal& portal, Position& position)
{
    if (auto destination = portal.destination.lock())
    {
        position.setPosition(portal.destination_pos, destination.get());
        return true;
    }
    return false;
}
