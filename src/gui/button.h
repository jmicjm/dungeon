#pragma once
#include "gui_element.h"
#include "surface.h"

#include "SFML/Graphics/Text.hpp"

#include <variant>
#include <functional>
#include <chrono>


namespace gui
{
    class Button : public Gui_element
    {
        Surface pressed_surface;
        Surface released_surface;
        Surface pressed_hovered_overlay;
        Surface released_hovered_overlay;

        sf::Text pressed_text;
        sf::Text released_text;

        std::function<void()> press_function;
        std::function<void()> release_function;

        std::chrono::milliseconds press_delay = std::chrono::milliseconds(100);
        std::chrono::steady_clock::time_point press_tp = std::chrono::steady_clock::now();

        bool isLocked();

        bool is_pressed = false;

    public:
        enum TYPE
        {
            PUSH, SWITCH
        };
    private:
        TYPE type = PUSH;

        void drawAction() override;

    public:
        using Gui_element::Gui_element;

        bool isPressed() const;

        void update() override;

        void setType(TYPE t);

        void setPressDelay(const std::chrono::milliseconds& delay);

        void setPressedSurface (const Surface& surface);
        void setReleasedSurface(const Surface& surface);
        void setPressedHoveredOverlay(const Surface& surface);
        void setReleasedHoveredOverlay(const Surface& surface);

        void setPressedText(const sf::Text& text);
        void setReleasedText(const sf::Text& text);

        void setPressFunction(const std::function<void()>& function);
        void setReleaseFunction(const std::function<void()>& function);
    };
}