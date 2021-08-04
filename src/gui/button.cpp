#include "button.h"
#include "SFML/Window/Mouse.hpp"

bool gui::Button::isPressed() const
{
    return is_pressed;
}

void gui::Button::update()
{
    if (is_pressed && type == PUSH)
    {
        is_pressed = false;
    }
    if (!isLocked() && isHovered() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        is_pressed = !is_pressed;
        press_tp = std::chrono::steady_clock::now();

        if (isPressed())
        {
            if (press_function) press_function();
        }
        else
        {
            if (release_function) release_function();
        }
    }
}

void gui::Button::setType(TYPE t)
{
    type = t;
}

void gui::Button::setPressDelay(const std::chrono::milliseconds& delay)
{
    press_delay = delay;
}

bool gui::Button::isLocked()
{
    return std::chrono::steady_clock::now() - press_tp <= press_delay;
}

void gui::Button::drawAction()
{  
    auto drawText = [&](sf::Text& text)
    {
        const sf::FloatRect tbounds = text.getLocalBounds();
        text.setOrigin(tbounds.left + tbounds.width / 2, tbounds.top + tbounds.height / 2);
        text.setPosition(getPosition() + getSize() / 2.f);
        window.draw(text);
    };

    auto draw = [&](Surface& surf)
    {
        surf.setPosition(getPosition());
        surf.setSize(getSize());
        window.draw(surf);
    };

    if (isPressed())
    {
        
        draw(pressed_surface);
        drawText(pressed_text);
    }
    else
    {
        draw(released_surface);
        drawText(released_text);
    }
    if (isHovered())
    {
        if (isPressed())
        {
            draw(pressed_hovered_overlay);
        }
        else
        {
            draw(released_hovered_overlay);
        }
    } 
}

void gui::Button::setPressedSurface(const Surface& surface)
{
    pressed_surface = surface;
}

void gui::Button::setReleasedSurface(const Surface& surface)
{
    released_surface = surface;
}

void gui::Button::setPressedHoveredOverlay(const Surface& surface)
{
    pressed_hovered_overlay = surface;
}

void gui::Button::setReleasedHoveredOverlay(const Surface& surface)
{
    released_hovered_overlay = surface;
}

void gui::Button::setPressedText(const sf::Text& text)
{
    pressed_text = text;
}

void gui::Button::setReleasedText(const sf::Text& text)
{
    released_text = text;
}

void gui::Button::setPressFunction(const std::function<void()>& function)
{
    press_function = function;
}

void gui::Button::setReleaseFunction(const std::function<void()>& function)
{
    release_function = function;
}
