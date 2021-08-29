#pragma once
#include "surface.h"

namespace gui
{
    struct Button_appearance
    {
        Surface pressed;
        Surface released;
        Surface pressed_hovered_overlay;
        Surface released_hovered_overlay;
    };
}