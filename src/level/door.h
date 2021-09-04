#pragma once
#include "../gfx/primitive_sprite.h"

class Door
{
   /* struct Door_sprite
    {
        Primitive_sprite bg, fg;
    };*/

    Primitive_sprite open_sprite;
    Primitive_sprite closed_sprite;

public:
    Door(Primitive_sprite open, Primitive_sprite closed);

    enum STATE
    {
        OPEN, CLOSED //,LOCKED
    };

    STATE state = CLOSED;

    Primitive_sprite getSprite() const;
};
