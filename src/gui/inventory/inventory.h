#pragma once
#include "../gui_component.h"
#include "../component_stack/component_stack_helper.h"
#include "../primitives/frame/frame.h"
#include "../primitives/text/text.h";
#include "../primitives/button/button.h"


namespace gui
{
    class Inventory : public Gui_component, public Component_stack_helper
    {
        gui::Frame bg;
        gui::Text text;
        gui::Button btn;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        Inventory();

        void update() override;
    };
}
