#pragma once
#include "position_info.h"
#include "size_info.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RenderTexture.hpp"


namespace gui
{
    class Gui_element
    {
        Position_info pos_info;
        Size_info size_info;   

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

        void setPositionInfo(const Position_info& p_info);
        Position_info getPositionInfo() const;
        sf::Vector2f getPosition() const;

        void setSizeInfo(const Size_info& s_info);
        Size_info getSizeInfo() const;
        sf::Vector2f getSize() const;
    };
}