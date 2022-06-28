#pragma once
#include "../../surface.h"

namespace gui
{
    struct Button_appearance
    {
        Surface pressed;
        Surface released;
        Surface pressed_hovered;
        Surface released_hovered;
    };
}