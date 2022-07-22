#include "button.h"
#include "../../../input/input.h"
#include "../../../asset_storage/font_bank.h"
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

        if (is_pressed && type == PUSH)
        {
            is_pressed = false;
        }
        is_hovered = Gui_component::isHovered();
        if (!isLocked() && isHovered() && Input::getMouseHoldVec(sf::Mouse::Button::Left))
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

    void Button::setText(sf::Text& text, const std::string& str)
    {
        text.setString(str);
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
            draw(isHovered() ? appearance.pressed_hovered : appearance.pressed);
            drawText(isHovered() ? pressed_hovered_text : pressed_text);
        }
        else
        {
            draw(isHovered() ? appearance.released_hovered : appearance.released);
            drawText(isHovered() ? released_hovered_text : released_text);
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

    void Button::setPressedText(const std::string& text)
    {
        setText(pressed_text, text);
    }

    void Button::setPressedHoveredText(const std::string& text)
    {
        setText(pressed_hovered_text, text);
    }

    void Button::setReleasedHoveredText(const std::string& text)
    {
        setText(released_hovered_text, text);
    }

    void Button::setReleasedText(const std::string& text)
    {
        setText(released_text, text);
    }

    void Button::setFont(const std::string& font)
    {
        this->font = *Font_bank::getFont(font);
        pressed_text.setFont(this->font);
        pressed_hovered_text.setFont(this->font);
        released_text.setFont(this->font);
        released_hovered_text.setFont(this->font);
    }

    void Button::setFontSize(float size)
    {
        pressed_text.setCharacterSize(size);
        pressed_hovered_text.setCharacterSize(size);
        released_text.setCharacterSize(size);
        released_hovered_text.setCharacterSize(size);
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