#include "frame.h"

namespace gui
{
    void Frame::redraw()
    {
        background.setSize(sf::Vector2f{ getSize() });
        draw(background);
        redraw_required = false;
    }

    bool Frame::isRedrawRequired() const
    {
        return redraw_required || background.hasChanged();
    }

    void Frame::setAppearance(const Frame_appearance& a)
    {
        background = a.background;
        background.setPosition({ 0,0 });
        redraw_required = true;
    }

    Frame_appearance Frame::getAppearance() const
    {
        return { background };
    }
}