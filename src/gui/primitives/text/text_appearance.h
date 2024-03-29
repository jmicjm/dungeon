#pragma once
#include "../scroll/scroll_appearance.h"
#include <string>

namespace gui
{
    struct Text_appearance
    {
        Scroll_appearance scroll;
        float scroll_width = 16;
        std::string font_name = "font.ttf";
        float font_size = 32;
        float letter_spacing = 1;
        float line_spacing = 0;
    };
}