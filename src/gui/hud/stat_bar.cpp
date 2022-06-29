#include "stat_bar.h"


void gui::Stat_bar::redraw()
{
    frame.draw();
    bar.draw();
}

gui::Stat_bar::Stat_bar(sf::RenderWindow* rw)
    : Gui_component(rw), frame(rw), bar(rw)
{
    frame.setSizeInfo({ {0,0}, {1,1} });

    bar.setSizeInfo({ {0,0}, {5.f/6, 6.f/10} });
    bar.setPositionInfo({ {0,0}, {2.f/60, 2.f/10} });

    frame.setParent(this);
    bar.setParent(this);
}

void gui::Stat_bar::setMinValue(float val)
{
    bar.setMinValue(val);
}

void gui::Stat_bar::setMaxValue(float val)
{
    bar.setMaxValue(val);
}

void gui::Stat_bar::setValue(float val)
{
    bar.setValue(val);
}

void gui::Stat_bar::setAppearance(const Frame_appearance& frame_a, const Gauge_bar_appearance& bar_a)
{
    frame.setAppearance(frame_a);
    bar.setAppearance(bar_a);
}
