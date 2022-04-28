#include "frame.h"

namespace gui
{
    void Frame::redraw()
    {
        background.setSize(sf::Vector2f{ size() });
        draw(background);
    }

    void Frame::setAppearance(const Frame_appearance& a)
    {
        background = a.background;
        background.setPosition({ 0,0 });
    }

    Frame_appearance Frame::getAppearance() const
    {
        return { background };
    }
}