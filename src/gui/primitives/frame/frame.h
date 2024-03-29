#pragma once
#include "../../gui_component.h"
#include "../../surface.h"
#include "frame_appearance.h"


namespace gui
{
    class Frame : public Gui_component
    {
        Surface background;

        void redraw() override;

    public:
        Frame();

        void setAppearance(const Frame_appearance& a);
        Frame_appearance getAppearance() const;
    };
}