#pragma once
#include "../gui/component_stack/component_stack.h"


inline gui::Component_stack gui_component_stack = []
{
    gui::Component_stack component_stack;
    component_stack.setSizeInfo({ {0,0}, {1,1} });
    component_stack.activate();
    return std::move(component_stack);
}();