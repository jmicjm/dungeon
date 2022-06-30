#pragma once
#include "../gui_component.h"
#include "../primitives/frame/frame.h"
#include "../primitives/text/text.h";


namespace gui
{
    class Inventory : public Gui_component
    {
        gui::Frame bg;
        gui::Text text;

        void redraw() override;
        void activateEvent() override;
        void deactivateEvent() override;

    public:
        Inventory();

        void update() override;
    };
}
