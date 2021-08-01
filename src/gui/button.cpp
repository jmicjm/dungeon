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
    auto drawTSprite = [&](auto& t_sprite)
    {
        t_sprite.setPosition(getPosition());
        const sf::Vector2f spr_size =
        {
            t_sprite.getGlobalBounds().width,
            t_sprite.getGlobalBounds().height
        };
        t_sprite.scale(getSize().x / spr_size.x, getSize().y / spr_size.y);
        window.draw(t_sprite);
    };

    auto drawSurface = [&](surface_type& surface)
    {
        sf::RectangleShape* rs = std::get_if<sf::RectangleShape>(&surface);
        if (rs != nullptr)
        {
            rs->setPosition(getPosition());
            rs->setSize(getSize());
            window.draw(*rs);
            return;
        }

        sf::Sprite* spr = std::get_if<sf::Sprite>(&surface);
        if (spr != nullptr)
        {
            drawTSprite(*spr);
            return;
        }

        Animated_sprite* aspr = std::get_if<Animated_sprite>(&surface);
        if (aspr != nullptr)
        {
            aspr->updateFrameIdx();
            drawTSprite(*aspr);
        }     
    };

    auto drawText = [&](sf::Text& text)
    {
        const sf::FloatRect tbounds = text.getLocalBounds();
        text.setOrigin(tbounds.left + tbounds.width / 2, tbounds.top + tbounds.height / 2);
        text.setPosition(getPosition() + getSize() / 2.f);
        window.draw(text);
    };

    if (isPressed())
    {
        drawSurface(pressed_surface);
        drawText(pressed_text);
    }
    else
    {
        drawSurface(released_surface);
        drawText(released_text);
    }
    if (isHovered())
    {
        if (isPressed())
        {
            drawSurface(pressed_hovered_overlay);
        }
        else
        {
            drawSurface(released_hovered_overlay);
        }
    } 
}

void gui::Button::setPressedSurface(const surface_type& surface)
{
    pressed_surface = surface;
}

void gui::Button::setReleasedSurface(const surface_type& surface)
{
    released_surface = surface;
}

void gui::Button::setPressedHoveredOverlay(const surface_type& surface)
{
    pressed_hovered_overlay = surface;
}

void gui::Button::setReleasedHoveredOverlay(const surface_type& surface)
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
