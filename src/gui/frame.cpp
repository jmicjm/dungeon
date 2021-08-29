#include "frame.h"

namespace gui
{
    void Frame::redraw()
    {
        background.setSize(sf::Vector2f{ getSize() });
        draw(background);
        redraw_required = false;
    }

    bool Frame::isRedrawRequired()
    {
        return redraw_required || background.hasChanged();
    }

    void Frame::setBackgroundSurface(const Surface& surface)
    {
        background = surface;
        background.setPosition({ 0,0 });
        redraw_required = true;
    }

    Surface Frame::getBackgroundSurface() const
    {
        return background;
    }
}