#include "door.h"

Door::Door(Primitive_sprite open, Primitive_sprite closed)
    :open_sprite(open), closed_sprite(closed) {}

Primitive_sprite Door::getSprite() const
{
    switch (state)
    {
    case OPEN:
        return open_sprite;
    case CLOSED:
        return closed_sprite;
    }
}
