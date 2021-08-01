#pragma once
#include "gui_element.h"
#include "../gfx/animated_sprite/animated_sprite.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

#include <variant>
#include <functional>
#include <chrono>


namespace gui
{
    class Button : public Gui_element
    {
        using surface_type = std::variant<sf::RectangleShape, sf::Sprite, Animated_sprite>;

        surface_type pressed_surface;
        surface_type released_surface;
        surface_type pressed_hovered_overlay;
        surface_type released_hovered_overlay;

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

        void setPressedSurface (const surface_type& surface);
        void setReleasedSurface(const surface_type& surface);
        void setPressedHoveredOverlay(const surface_type& surface);
        void setReleasedHoveredOverlay(const surface_type& surface);

        void setPressedText(const sf::Text& text);
        void setReleasedText(const sf::Text& text);

        void setPressFunction(const std::function<void()>& function);
        void setReleaseFunction(const std::function<void()>& function);
    };
}