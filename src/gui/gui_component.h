#pragma once
#include "position_info.h"
#include "size_info.h"
#include "anchor_position_info.h"

#include "SFML/Graphics/RenderWindow.hpp"

#include <functional>


namespace gui
{
    class Gui_component
    {
        sf::RenderWindow* window;

        sf::Vector2f old_size = { 0,0 };

        Position_info pos_info;
        Size_info size_info;   

        const Gui_component* anchor = nullptr;
        Anchor_position_info anchor_pos_info;

        const Gui_component* parent = nullptr;
        sf::Vector2f parentGlobalPosition() const;
        sf::Vector2f parentSize() const;
        sf::FloatRect parentArea() const;
        sf::FloatRect visibleParentArea() const;

        std::function<sf::Vector2f(sf::Vector2f)> pos_function;
        std::function<sf::Vector2f(sf::Vector2f)> size_function;

        bool is_active = false;
        bool limit_to_parent = true;       

    protected:  
        template<typename ...T>
        void draw(T&&... args);

        virtual void redraw() = 0;

        virtual void resizeEvent(sf::Vector2f size_diff) {}
        virtual void activateEvent() {}
        virtual void deactivateEvent() {}

    public:
        Gui_component(sf::RenderWindow* rw);
        virtual ~Gui_component() = default;

        void draw();
        virtual void update() {}

        void activate();
        void deactivate();
        bool isActive() const;

        bool isHovered() const;
        sf::FloatRect area() const;
        sf::FloatRect visibleArea() const;

        sf::Vector2f size() const;

        sf::Vector2f position() const;
        sf::Vector2f globalPosition() const;

        void setPositionInfo(Position_info p_info);
        Position_info getPositionInfo() const;

        void setSizeInfo(Size_info s_info);
        Size_info getSizeInfo() const;

        void setAnchor(const Gui_component* anchor);
        const Gui_component* getAnchor() const;
        void setAnchorPositionInfo(Anchor_position_info a_info);
        Anchor_position_info getAnchorPositionInfo() const;

        void setParent(const Gui_component* parent);
        const Gui_component* getParent() const;
        void limitToParent(bool limit);
        bool isLimitedToParent() const;

        void setPositionFunction(std::function<sf::Vector2f(sf::Vector2f)> func);
        std::function<sf::Vector2f(sf::Vector2f)> getPositionFunction() const;
        void setSizeFunction(std::function<sf::Vector2f(sf::Vector2f)> func);
        std::function<sf::Vector2f(sf::Vector2f)> getSizeFunction() const;
    };

    template<typename ...T>
    void Gui_component::draw(T&&... args)
    {
        window->draw(std::forward<T>(args)...);
    }
}