#include "gauge_bar.h"

#include <algorithm>
#include <cmath>


namespace gui
{
    void Gauge_bar::drawAction()
    {
        if (getSize() != prev_size)
        {
            cached_tex.create(getSize().x, getSize().y);
        }
        prev_size = getSize();
        cached_tex.clear({ 0,0,0,255 });

        const float percentage = (current_value-min_value)/std::abs(min_value - max_value);

        bar_surface.setSize(getSize());
        cached_tex.draw(bar_surface);

        sf::RectangleShape rect;
        rect.setFillColor({ 0,0,0,0 });

        switch (direction)
        {
        case L_TO_R:
        case R_TO_L:
            rect.setSize({ getSize().x * (1 - percentage), getSize().y });      
            break;
        case T_TO_B:
        case B_TO_T:
            rect.setSize({ getSize().x, getSize().y * (1 - percentage) });
        }

        switch (direction)
        {
        case L_TO_R:
            rect.setPosition({ getSize().x - rect.getSize().x, 0 });
            break;
        case T_TO_B:
            rect.setPosition({ 0, getSize().y - rect.getSize().y });
            break;
        case R_TO_L:
        case B_TO_T:
            rect.setPosition({ 0, 0 });
        }

        cached_tex.draw(rect, sf::BlendMultiply);
        cached_tex.display();

        sf::Sprite bar_sprite(cached_tex.getTexture());
        bar_sprite.setPosition(getPosition());
        window.draw(bar_sprite);
    }

    void Gauge_bar::setDirection(DIRECTION dir)
    {
        direction = dir;
    }

    void Gauge_bar::setMinValue(float val)
    {
        min_value = val;
        max_value = std::max(max_value, min_value);
    }

    float Gauge_bar::getMinValue() const
    {
        return min_value;
    }

    void Gauge_bar::setMaxValue(float val)
    {
        max_value = val;
        min_value = std::min(min_value, max_value);
    }

    float Gauge_bar::getMaxValue() const
    {
        return max_value;
    }

    void Gauge_bar::setValue(float val)
    {
        current_value = std::clamp(val, min_value, max_value);
    }

    float Gauge_bar::getValue() const
    {
        return current_value;
    }

    void Gauge_bar::setBarSurface(const Surface& surf)
    {
        bar_surface = surf;
        bar_surface.setPosition({ 0,0 });
    }
}