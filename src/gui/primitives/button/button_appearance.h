#pragma once
#include "../../surface.h"
#include "../apperance_common.h"
#include <string>

namespace gui
{
    struct Button_appearance
    {
        Surface pressed = rect({ 255, 192, 192, 255 });
        Surface released = rect({ 245, 203, 66, 255 });
        Surface pressed_hovered = rect({ 173, 144, 49, 255 });
        Surface released_hovered = rect({ 209, 173, 54, 255 });
        std::string font_name = "FreeMono.ttf";
        float font_size = 24;
    };
}
