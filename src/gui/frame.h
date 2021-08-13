#pragma once
#include "gui_element.h"
#include "surface.h"


namespace gui
{
    class Frame : public Gui_element
    {
        Surface background;

        bool redraw_required = true;

        void redraw(const sf::Vector2i& size_diff) override;
        bool isRedrawRequired() override;

    public:
        using Gui_element::Gui_element;

        void setBackgroundSurface(const Surface& surface);
        Surface getBackgroundSurface() const;
    };
}