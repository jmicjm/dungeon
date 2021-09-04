#pragma once
#include "surface.h"
#include "button_appearance.h"
#include "frame_appearance.h"

namespace gui
{
    struct Scroll_appearance
    {
        float button_fixed_height = 0;
        float button_percentage_height = 0;
        Button_appearance top_button;
        Button_appearance bottom_button;
        Frame_appearance handle;
        Frame_appearance line;
    };
}