#pragma once
#include "../../surface.h"
#include "../button/button_appearance.h"
#include "../frame/frame_appearance.h"
#include "../apperance_common.h"

namespace gui
{
    struct Scroll_appearance
    {
        float button_fixed_height = 16;
        float button_percentage_height = 0;
        Button_appearance top_button;
        Button_appearance bottom_button;
        Frame_appearance handle = { rect({ 173, 144, 49, 255 }) };
        Frame_appearance line = { rect({ 69, 57, 19, 255 }) };
    };
}