#pragma once
#include "gui_component.h"
#include "frame_appearance.h"
#include "surface.h"


namespace gui
{
    class Frame : public Gui_component
    {
        Surface background;

        bool redraw_required = true;

        void redraw() override;

    public:
        using Gui_component::Gui_component;

        bool isRedrawRequired() const override;

        void setAppearance(const Frame_appearance& a);
        Frame_appearance getAppearance() const;
    };
}