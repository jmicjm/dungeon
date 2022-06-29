#pragma once
#include "../gui_component.h"
#include "../primitives/gauge_bar/gauge_bar.h"
#include "../primitives/frame/frame.h"

namespace gui
{
    class Stat_bar : public Gui_component
    {
        Frame frame;
        Gauge_bar bar;

        void redraw() override;

    public:
        Stat_bar(sf::RenderWindow* rw);

        void setMinValue(float val);
        void setMaxValue(float val);
        void setValue(float val);

        void setAppearance(const Frame_appearance& frame_a, const Gauge_bar_appearance& bar_a);
    };
}