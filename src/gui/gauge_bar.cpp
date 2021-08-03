#include "gauge_bar.h"

#include "SFML/Graphics/RenderTexture.hpp"

#include <algorithm>
#include <cmath>


namespace gui
{
    void Gauge_bar::drawAction()
    {
        sf::RenderTexture rtex;
        rtex.create(getSize().x, getSize().y);
        rtex.clear({ 0,0,0,255 });

        const float percentage = (current_value-min_value)/std::abs(min_value - max_value);

        bar_surface.setSize(getSize());
        bar_surface.draw(rtex);

        sf::RectangleShape rect;
        rect.setFillColor({ 0,0,0,0 });
        rect.setSize({ getSize().x * (1 - percentage), getSize().y });
        rect.setPosition({ getSize().x - rect.getSize().x, 0 });

        rtex.draw(rect, sf::BlendMultiply);
        rtex.display();

        sf::Sprite bar_sprite(rtex.getTexture());
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