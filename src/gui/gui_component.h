#pragma once
#include "position_info.h"
#include "size_info.h"
#include "anchor_position_info.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RenderTexture.hpp"

#include <functional>


namespace gui
{
    class Gui_component
    {
        sf::RenderWindow* window;
        sf::RenderTexture rtex;

        Position_info pos_info;
        Size_info size_info;   

        const Gui_component* anchor = nullptr;
        Anchor_position_info anchor_pos_info;

        const Gui_component* parent = nullptr;
        sf::Vector2f getParentGlobalPosition() const;
        sf::Vector2i getParentSize() const;

        std::function<sf::Vector2f()> pos_function;
        std::function<sf::Vector2i()> size_function;

        void updateTex();

        bool is_active = false;

    protected:       
        void draw(sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
        void draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
        void draw(Gui_component& component);
        virtual void redraw() = 0;

        virtual void resizeEvent(const sf::Vector2i& size_diff) {}
        virtual void activateEvent() {}
        virtual void deactivateEvent() {}

    public:
        Gui_component(sf::RenderWindow* rw);
        Gui_component(const Gui_component& other);
        Gui_component(Gui_component&& other) noexcept;
        Gui_component& operator=(const Gui_component& other);
        Gui_component& operator=(Gui_component&& other) noexcept;
        virtual ~Gui_component() = default;

        void draw();
        virtual bool isRedrawRequired() const;
        virtual void update() {}
        void activate();
        void deactivate();
        bool isActive() const;

        bool isHovered() const;

        void setPositionInfo(Position_info p_info);
        Position_info getPositionInfo() const;
        sf::Vector2f getPosition() const;
        sf::Vector2f getGlobalPosition() const;

        void setSizeInfo(Size_info s_info);
        Size_info getSizeInfo() const;
        sf::Vector2i getSize() const;

        void setAnchor(const Gui_component* anchor);
        const Gui_component* getAnchor() const;
        void setAnchorPositionInfo(Anchor_position_info a_info);
        Anchor_position_info getAnchorPositionInfo() const;

        void setParent(const Gui_component* parent);
        const Gui_component* getParent() const;

        void setPositionFunction(std::function<sf::Vector2f()> func);
        std::function<sf::Vector2f()> getPositionFunction() const;
        void setSizeFunction(std::function<sf::Vector2i()> func);
        std::function<sf::Vector2i()> getSizeFunction() const;
    };
}