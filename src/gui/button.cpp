#include "button.h"
#include "SFML/Window/Mouse.hpp"

bool gui::Button::isPressed() const
{
    return is_pressed;
}

void gui::Button::update()
{
    const bool was_pressed = is_pressed;
    const bool was_hovered = is_hovered;

    if (is_pressed && type == PUSH)
    {
        is_pressed = false;
    }
    is_hovered = Gui_element::isHovered();
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
    if (is_pressed != was_pressed || is_hovered != was_hovered) redraw_required = true;
}

void gui::Button::setType(TYPE t)
{
    type = t;
}

void gui::Button::setPressDelay(const std::chrono::milliseconds& delay)
{
    press_delay = delay;
}

bool gui::Button::isLocked() const
{
    return std::chrono::steady_clock::now() - press_tp <= press_delay;
}

bool gui::Button::isHovered() const
{
    return is_hovered;
}

void gui::Button::redraw(const sf::Vector2i& size_diff)
{  
    auto drawText = [&](sf::Text& text)
    {
        const sf::FloatRect tbounds = text.getLocalBounds();
        text.setOrigin(tbounds.left + tbounds.width / 2, tbounds.top + tbounds.height / 2);
        text.setPosition(sf::Vector2f{ getSize() } / 2.f);
        draw(text);
    };

    auto draw = [&](Surface& surf)
    {
        surf.setSize(sf::Vector2f{ getSize() });
        Gui_element::draw(surf);
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
    redraw_required = false;
}

bool gui::Button::isRedrawRequired()
{
    const bool surf_req = (isPressed() ? pressed_surface : released_surface).hasChanged();
    const bool hover_req = isHovered() ? (isPressed() ? pressed_hovered_overlay : released_hovered_overlay).hasChanged() : false;

    return redraw_required || surf_req || hover_req;
}

void gui::Button::setPressedSurface(const Surface& surface)
{
    pressed_surface = surface;
    redraw_required = true;
}

void gui::Button::setReleasedSurface(const Surface& surface)
{
    released_surface = surface;
    redraw_required = true;
}

void gui::Button::setPressedHoveredOverlay(const Surface& surface)
{
    pressed_hovered_overlay = surface;
    redraw_required = true;
}

void gui::Button::setReleasedHoveredOverlay(const Surface& surface)
{
    released_hovered_overlay = surface;
    redraw_required = true;
}

void gui::Button::setPressedText(const sf::Text& text)
{
    pressed_text = text;
    redraw_required = true;
}

void gui::Button::setReleasedText(const sf::Text& text)
{
    released_text = text;
    redraw_required = true;
}

void gui::Button::setPressFunction(const std::function<void()>& function)
{
    press_function = function;
}

void gui::Button::setReleaseFunction(const std::function<void()>& function)
{
    release_function = function;
}
