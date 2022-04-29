#include "quick_select.h"

void gui::Quick_select::redraw()
{
    frame.draw();
}

gui::Quick_select::Quick_select(sf::RenderWindow* rw)
    : Gui_component(rw), frame(rw)
{
    frame.setSizeInfo({ {0,0}, {1,1} });
    frame.setParent(this);
}

void gui::Quick_select::setAppearance(const Frame_appearance& appearance)
{
    frame.setAppearance(appearance);
}