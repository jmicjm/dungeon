#pragma once
#include "../../gui_component.h"
#include "../../surface.h"
#include "button_appearance.h"

#include "SFML/Graphics/Text.hpp"

#include <functional>
#include <chrono>


namespace gui
{
    class Button : public Gui_component
    {
        Button_appearance appearance;

        sf::Text pressed_text;
        sf::Text released_text;

        std::function<void()> press_function;
        std::function<void()> release_function;

        std::chrono::milliseconds press_delay = std::chrono::milliseconds(100);
        std::chrono::steady_clock::time_point press_tp = std::chrono::steady_clock::now();

        bool isLocked() const;

        bool is_pressed = false;
        bool is_hovered = false;

        bool isHovered() const;

    public:
        enum TYPE
        {
            PUSH, SWITCH
        };
    private:
        TYPE type = PUSH;

        void redraw() override;

        void deactivateEvent() override;

    public:
        using Gui_component::Gui_component;

        bool isPressed() const;

        void update() override;

        void setType(TYPE t);

        void setPressDelay(const std::chrono::milliseconds& delay);

        void setAppearance(const Button_appearance& a);
        Button_appearance getAppearance() const;

        void setPressedText(const sf::Text& text);
        void setReleasedText(const sf::Text& text);

        void setPressFunction(const std::function<void()>& function);
        void setReleaseFunction(const std::function<void()>& function);
    };
}