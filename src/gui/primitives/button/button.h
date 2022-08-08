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
        sf::Text pressed_hovered_text;
        sf::Text released_text;
        sf::Text released_hovered_text;
        sf::Font font;

        std::function<void()> press_function;
        std::function<void()> release_function;

        std::chrono::milliseconds press_delay = std::chrono::milliseconds(100);
        std::chrono::steady_clock::time_point press_tp = std::chrono::steady_clock::now();

        bool isLocked() const;

        bool is_pressed = false;
        bool is_hovered = false;

        bool isHovered() const;

        void setText(sf::Text& text, const std::string& str);

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
        Button();

        bool isPressed() const;

        void update() override;

        void setType(TYPE t);

        void setPressDelay(const std::chrono::milliseconds& delay);

        void setAppearance(const Button_appearance& a);
        Button_appearance getAppearance() const;

        void setPressedText(const std::string& text);
        void setPressedHoveredText(const std::string& text);
        void setReleasedHoveredText(const std::string& text);
        void setReleasedText(const std::string& text);

        void setFont(const std::string& font);
        void setFontSize(float size);

        void setPressFunction(const std::function<void()>& function);
        void setReleaseFunction(const std::function<void()>& function);
    };
}