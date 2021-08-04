#pragma once
#include "position_info.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RenderTexture.hpp"


namespace gui
{
    class Gui_element
    {
        Position_info pos_info;
        sf::Vector2f size;   

        sf::RenderWindow& window;
        sf::RenderTexture rtex;

    protected:       
        void draw(sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
        virtual void redraw(const sf::Vector2i& size_diff) = 0;
        virtual bool isRedrawRequired();

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