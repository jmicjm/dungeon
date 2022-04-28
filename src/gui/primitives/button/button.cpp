#include "button.h"
#include "SFML/Window/Mouse.hpp"


namespace gui
{
    bool Button::isPressed() const
    {
        return is_pressed;
    }

    void Button::update()
    {
        if (!isActive()) return;

        const bool was_pressed = is_pressed;
        const bool was_hovered = is_hovered;

        if (is_pressed && type == PUSH)
        {
            is_pressed = false;
        }
        is_hovered = Gui_component::isHovered();
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

    void Button::setType(TYPE t)
    {
        type = t;
    }

    void Button::setPressDelay(const std::chrono::milliseconds& delay)
    {
        press_delay = delay;
    }

    bool Button::isLocked() const
    {
        return std::chrono::steady_clock::now() - press_tp <= press_delay;
    }

    bool Button::isHovered() const
    {
        return is_hovered;
    }

    void Button::redraw()
    {
        auto drawText = [&](sf::Text& text)
        {
            const sf::FloatRect tbounds = text.getLocalBounds();
            text.setOrigin(tbounds.left + tbounds.width / 2, tbounds.top + tbounds.height / 2);
            text.setPosition(sf::Vector2f{ size() } / 2.f);
            draw(text);
        };

        auto draw = [&](Surface& surf)
        {
            surf.setSize(sf::Vector2f{ size() });
            surf.setPosition({ 0,0 });
            Gui_component::draw(surf);
        };

        if (isPressed())
        {
            draw(appearance.pressed);
            drawText(pressed_text);
        }
        else
        {
            draw(appearance.released);
            drawText(released_text);
        }
        if (isHovered())
        {
            if (isPressed())
            {
                draw(appearance.pressed_hovered_overlay);
            }
            else
            {
                draw(appearance.released_hovered_overlay);
            }
        }
    }

    void Button::deactivateEvent()
    {
        is_hovered = false;
        if (type == PUSH && std::exchange(is_pressed, false) && release_function) release_function();
    }

    void Button::setAppearance(const Button_appearance& a)
    {
        appearance = a;
    }

    Button_appearance Button::getAppearance() const
    {
        return appearance;
    }

    void Button::setPressedText(const sf::Text& text)
    {
        pressed_text = text;
    }

    void Button::setReleasedText(const sf::Text& text)
    {
        released_text = text;
    }

    void Button::setPressFunction(const std::function<void()>& function)
    {
        press_function = function;
    }

    void Button::setReleaseFunction(const std::function<void()>& function)
    {
        release_function = function;
    }

}