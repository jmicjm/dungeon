#pragma once
#include "position_info.h"
#include "size_info.h"
#include "anchor_position_info.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RenderTexture.hpp"


namespace gui
{
    class Gui_element
    {
        sf::RenderWindow& window;
        sf::RenderTexture rtex;

        Position_info pos_info;
        Size_info size_info;   

        const Gui_element* anchor = nullptr;
        Anchor_position_info anchor_pos_info;

        const Gui_element* parent = nullptr;
        sf::Vector2f getParentPosition() const;
        sf::Vector2i getParentSize() const;

        void updateTex();

    protected:       
        void draw(sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
        void draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
        void draw(Gui_element& element, bool u = true);
        virtual void redraw(const sf::Vector2i& size_diff) = 0;

        virtual void resizeEvent() {}

    public:
        Gui_element(sf::RenderWindow& rw);
        virtual ~Gui_element() = default;

        void draw(bool update = true);
        virtual void update() {}
        virtual bool isRedrawRequired();

        bool isHovered() const;

        void setPositionInfo(Position_info p_info);
        Position_info getPositionInfo() const;
        sf::Vector2f getPosition() const;
        sf::Vector2f getGlobalPosition() const;

        void setSizeInfo(Size_info s_info);
        Size_info getSizeInfo() const;
        sf::Vector2i getSize() const;

        void setAnchor(const Gui_element* a);
        const Gui_element* getAnchor() const;
        void setAnchorPositionInfo(Anchor_position_info a_info);
        Anchor_position_info getAnchorPositionInfo() const;

        void setParent(const Gui_element* p);
        const Gui_element* getParent() const;
    };
}