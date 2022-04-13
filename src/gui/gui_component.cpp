#include "gui_component.h"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include <cmath>
#include <algorithm>

namespace gui
{
    sf::Vector2f Gui_component::parentGlobalPosition() const
    {
        return parent ? parent->globalPosition() : sf::Vector2f{0,0};
    }
    sf::Vector2i Gui_component::parentSize() const
    {
        return parent ? parent->size() : sf::Vector2i{ window->getSize() };
    }
    sf::FloatRect Gui_component::parentArea() const
    {
        return parent ? parent->area() : sf::FloatRect{ { 0,0 }, sf::Vector2f{ window->getSize() } };
    }
    sf::FloatRect Gui_component::visibleParentArea() const
    {
        return parent ? parent->visibleArea() : sf::FloatRect{ { 0,0 }, sf::Vector2f{ window->getSize() } };
    }
    void Gui_component::updateTex()
    {
        const sf::Vector2i size_diff = sf::Vector2i{ size() } - sf::Vector2i{ rtex.getSize() };
        if (size_diff != sf::Vector2i{ 0,0 })
        {
            rtex.create(size().x, size().y);
            resizeEvent(size_diff);
        }
        if (isRedrawRequired() || size_diff != sf::Vector2i{ 0,0 })
        {
            rtex.clear({ 0,0,0,0 });
            redraw();
            rtex.display();
        }
    }
    void Gui_component::draw(sf::Drawable& drawable, const sf::RenderStates& states)
    {
        rtex.draw(drawable, states);
    }

    void Gui_component::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
    {
        rtex.draw(vertices, vertexCount, type, states);
    }

    void Gui_component::draw(Gui_component& component)
    {
        component.updateTex();
        sf::Sprite elem_sprite(component.rtex.getTexture());
        elem_sprite.setPosition(component.position());
        rtex.draw(elem_sprite);
    }

    bool Gui_component::isRedrawRequired() const
    {
        return true;
    }

    void Gui_component::activate()
    {
        is_active = true;
        activateEvent();
    }

    void Gui_component::deactivate()
    {
        is_active = false;
        deactivateEvent();
    }

    bool Gui_component::isActive() const
    {
        return is_active;
    }

    Gui_component::Gui_component(sf::RenderWindow* rw)
        : window(rw) {}

    Gui_component::Gui_component(const Gui_component& other)
        : window(other.window),
        pos_info(other.pos_info),
        size_info(other.size_info),
        anchor(other.anchor),
        anchor_pos_info(other.anchor_pos_info),
        parent(other.parent) {}

    Gui_component::Gui_component(Gui_component&& other) noexcept
        : window(std::move(other.window)),
        pos_info(std::move(other.pos_info)),
        size_info(std::move(other.size_info)),
        anchor(std::move(other.anchor)),
        anchor_pos_info(std::move(other.anchor_pos_info)),
        parent(std::move(other.parent)) {}

    Gui_component& Gui_component::operator=(const Gui_component& other)
    {
        window = other.window;
        pos_info = other.pos_info;
        size_info = other.size_info;
        anchor = other.anchor;
        anchor_pos_info = other.anchor_pos_info;
        parent = other.parent;

        rtex.~RenderTexture();
        new(&rtex) sf::RenderTexture;

        return *this;
    }

    Gui_component& Gui_component::operator=(Gui_component&& other) noexcept
    {
        window = std::move(other.window);
        pos_info = std::move(other.pos_info);
        size_info = std::move(other.size_info);
        anchor = std::move(other.anchor);
        anchor_pos_info = std::move(other.anchor_pos_info);
        parent = std::move(other.parent);

        rtex.~RenderTexture();
        new(&rtex) sf::RenderTexture;

        return *this;
    }

    void Gui_component::draw() 
    {
        const sf::View old_view = window->getView();
        const sf::View view{ sf::FloatRect{ 0,0, window->getSize().x * 1.f, window->getSize().y * 1.f } };
        window->setView(view);

        updateTex();

        sf::Sprite elem_sprite(rtex.getTexture());
        elem_sprite.setPosition(globalPosition());
        window->draw(elem_sprite);

        window->setView(old_view);
    }

    bool Gui_component::isHovered() const
    {
        return visibleArea().contains(sf::Vector2f{ sf::Mouse::getPosition(*window) });
    }

    sf::FloatRect Gui_component::area() const
    {
        return { globalPosition(), sf::Vector2f{ size() } };
    }

    sf::FloatRect Gui_component::visibleArea() const
    {
        auto clipRect = [](const sf::FloatRect& inner, const sf::FloatRect& outer) -> sf::FloatRect
        {
            if (!inner.intersects(outer)) return { 0,0,0,0 };

            const sf::Vector2f tl = { std::max(inner.left, outer.left),
                                      std::max(inner.top, outer.top) };
            const sf::Vector2f br = { std::min(inner.left + inner.width, outer.left + outer.width),
                                      std::min(inner.top + inner.height, outer.top + outer.height) };
            return { tl, br - tl };
        };

        return clipRect(area(), visibleParentArea());
    }

    sf::Vector2f Gui_component::position() const
    {
        if (pos_function)
        {
            return pos_function(parentSize());
        }
        else if (anchor)
        {
            const auto& [side, offset, relative_to] = anchor_pos_info;

            sf::Vector2f pos = anchor->position() + offset;

            switch (side)
            {
            case Anchor_position_info::TOP:
            case Anchor_position_info::BOTTOM:
                pos.x += anchor->size().x * relative_to - size().x * relative_to;
                break;
            case Anchor_position_info::LEFT:
            case Anchor_position_info::RIGHT:
                pos.y += anchor->size().y * relative_to - size().y * relative_to;
            }

            switch (side)
            {
            case Anchor_position_info::TOP:
                pos.y -= size().y;
                break;
            case Anchor_position_info::BOTTOM:
                pos.y += anchor->size().y;
                break;
            case Anchor_position_info::LEFT:
                pos.x -= size().x;
                break;
            case Anchor_position_info::RIGHT:
                pos.x += anchor->size().x;
            }

            return pos;
        }
        else
        {
            const sf::Vector2i ps = parentSize();
            const auto& [off, poff, rel] = pos_info;
            const sf::Vector2f pos =
            {
                off.x + poff.x * ps.x + ps.x * rel.x - size().x * rel.x,
                off.y + poff.y * ps.y + ps.y * rel.y - size().y * rel.y
            };

            return pos;
        }
    }

    sf::Vector2f Gui_component::globalPosition() const
    {
        return parentGlobalPosition() + position();
    }

    void Gui_component::setPositionInfo(Position_info p_info)
    {
        pos_info = p_info;
    }

    Position_info Gui_component::getPositionInfo() const
    {
        return pos_info;
    }

    void Gui_component::setSizeInfo(Size_info s_info)
    {
        size_info = s_info;
    }

    Size_info Gui_component::getSizeInfo() const
    {
        return size_info;
    }

    sf::Vector2i Gui_component::size() const
    {
        const sf::Vector2i size = [&]() -> sf::Vector2i
        {
            if (size_function)
            {
                return size_function(parentSize());
            }
            else
            {
                const auto& p = size_info.percentage;
                const auto  s = parentSize();
                return { static_cast<int>(std::round(size_info.fixed.x + s.x * p.x)),
                         static_cast<int>(std::round(size_info.fixed.y + s.y * p.y)) };
            }
        }();

        return { std::max(0, size.x),
                 std::max(0, size.y) };
    }

    void Gui_component::setAnchor(const Gui_component* anchor)
    {
        this->anchor = anchor;
    }

    const Gui_component* Gui_component::getAnchor() const
    {
        return anchor;
    }

    void Gui_component::setAnchorPositionInfo(Anchor_position_info a_info)
    {
        anchor_pos_info = a_info;
    }

    Anchor_position_info Gui_component::getAnchorPositionInfo() const
    {
        return anchor_pos_info;
    }
    void Gui_component::setParent(const Gui_component* parent)
    {
        this->parent = parent;
    }
    const Gui_component* Gui_component::getParent() const
    {
        return parent;
    }
    void Gui_component::setPositionFunction(std::function<sf::Vector2f(sf::Vector2i)> func)
    {
        pos_function = func;
    }
    std::function<sf::Vector2f(sf::Vector2i)> Gui_component::getPositionFunction() const
    {
        return pos_function;
    }
    void Gui_component::setSizeFunction(std::function<sf::Vector2i(sf::Vector2i)> func)
    {
        size_function = func;
    }
    std::function<sf::Vector2i(sf::Vector2i)> Gui_component::getSizeFunction() const
    {
        return size_function;
    }
}
