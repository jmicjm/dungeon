#pragma once
#include "../gui_component.h"
#include "../primitives/frame/frame.h"


namespace gui
{
    class Quick_select : public Gui_component
    {
        Frame frame;

        void redraw() override;

    public:
        Quick_select(sf::RenderWindow* rw);

        void setAppearance(const Frame_appearance& appearance);
    };
}