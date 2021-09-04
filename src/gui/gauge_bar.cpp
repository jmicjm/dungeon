#include "gauge_bar.h"

#include <algorithm>
#include <cmath>


namespace gui
{
    void Gauge_bar::redraw()
    {
        const float percentage = (current_value-min_value)/std::abs(min_value - max_value);

        bar_surface.setSize(sf::Vector2f{ getSize() });
        draw(bar_surface);

        sf::RectangleShape rect;
        rect.setFillColor({ 0,0,0,0 });

        switch (direction)
        {
        case L_TO_R:
        case R_TO_L:
            rect.setSize(sf::Vector2f( getSize().x * (1 - percentage), getSize().y ));      
            break;
        case T_TO_B:
        case B_TO_T:
            rect.setSize(sf::Vector2f( getSize().x, getSize().y * (1 - percentage) ));
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

        draw(rect, sf::BlendMultiply);

        redraw_required = false;
    }

    bool Gauge_bar::isRedrawRequired() const
    {
        return redraw_required || bar_surface.hasChanged();
    }

    void Gauge_bar::setDirection(DIRECTION dir)
    {
        direction = dir;
        redraw_required = true;
    }

    void Gauge_bar::setMinValue(float val)
    {
        min_value = val;
        max_value = std::max(max_value, min_value);
        redraw_required = true;
    }

    float Gauge_bar::getMinValue() const
    {
        return min_value;
    }

    void Gauge_bar::setMaxValue(float val)
    {
        max_value = val;
        min_value = std::min(min_value, max_value);
        redraw_required = true;
    }

    float Gauge_bar::getMaxValue() const
    {
        return max_value;
    }

    void Gauge_bar::setValue(float val)
    {
        current_value = std::clamp(val, min_value, max_value);
        redraw_required = true;
    }

    float Gauge_bar::getValue() const
    {
        return current_value;
    }

    void Gauge_bar::setAppearance(const Gauge_bar_appearance& a)
    {
        bar_surface = a.bar;
        bar_surface.setPosition({ 0,0 });
        redraw_required = true;
    }

    Gauge_bar_appearance Gauge_bar::getAppearance() const
    {
        return { bar_surface };
    }
}