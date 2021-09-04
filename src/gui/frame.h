#pragma once
#include "gui_element.h"
#include "frame_appearance.h"
#include "surface.h"


namespace gui
{
    class Frame : public Gui_element
    {
        Surface background;

        bool redraw_required = true;

        void redraw() override;

    public:
        using Gui_element::Gui_element;

        bool isRedrawRequired() const override;

        void setAppearance(const Frame_appearance& a);
        Frame_appearance getAppearance() const;
    };
}