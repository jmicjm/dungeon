#pragma once
#include "position_info.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

namespace gui
{
    class Gui_element
    {
        Position_info pos_info;
        sf::Vector2f size;   
    protected:
        sf::RenderWindow& window;

        virtual void drawAction() = 0;

    public:
        Gui_element(sf::RenderWindow& rw);

        void draw(bool update = true);
        virtual void update() {}

        bool isHovered() const;

        sf::Vector2f getPosition() const;
        void setPositionInfo(const Position_info& p_info);
        Position_info getPositionInfo() const;
        void setSize(const sf::Vector2f& s);
        sf::Vector2f getSize() const;
    };
}