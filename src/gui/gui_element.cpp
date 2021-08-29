#include "gui_element.h"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include <cmath>

namespace gui
{
    sf::Vector2f Gui_element::getParentPosition() const
    {
        return parent ? parent->getGlobalPosition() : sf::Vector2f{0,0};
    }
    sf::Vector2i Gui_element::getParentSize() const
    {
        return parent ? parent->getSize() : sf::Vector2i{ window.getSize() };
    }
    void Gui_element::updateTex()
    {
        const sf::Vector2i size_diff = sf::Vector2i{ getSize() } - sf::Vector2i{ rtex.getSize() };
        if (size_diff != sf::Vector2i{ 0,0 })
        {
            rtex.create(getSize().x, getSize().y);
            resizeEvent(size_diff);
        }
        if (isRedrawRequired() || size_diff != sf::Vector2i{ 0,0 })
        {
            rtex.clear({ 0,0,0,0 });
            redraw();
            rtex.display();
        }
    }
    void Gui_element::draw(sf::Drawable& drawable, const sf::RenderStates& states)
    {
        rtex.draw(drawable, states);
    }

    void Gui_element::draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states)
    {
        rtex.draw(vertices, vertexCount, type, states);
    }

    void Gui_element::draw(Gui_element& element, bool u)
    {
        if (u) element.update();

        element.updateTex();
        sf::Sprite elem_sprite(element.rtex.getTexture());
        elem_sprite.setPosition(element.getPosition());
        rtex.draw(elem_sprite);
    }

    bool Gui_element::isRedrawRequired()
    {
        return true;
    }

    Gui_element::Gui_element(sf::RenderWindow& rw)
        : window(rw) {}

    void Gui_element::draw(bool u)
    {
        if (u) update();

        const sf::View old_view = window.getView();
        const sf::View view{ sf::FloatRect{ 0,0, window.getSize().x * 1.f, window.getSize().y * 1.f } };
        window.setView(view);

        updateTex();

        sf::Sprite elem_sprite(rtex.getTexture());
        elem_sprite.setPosition(getGlobalPosition());
        window.draw(elem_sprite);

        window.setView(old_view);
    }

    bool Gui_element::isHovered() const
    {
        const sf::Vector2i mp = sf::Mouse::getPosition(window);
        const sf::Vector2f tl = getGlobalPosition();
        const sf::Vector2f br = tl + sf::Vector2f{ getSize() };
        return mp.x >= tl.x && mp.x <= br.x 
            && mp.y >= tl.y && mp.y <= br.y;
    }

    sf::Vector2f Gui_element::getPosition() const
    {
        if (!anchor)
        {
            const sf::Vector2i ps = getParentSize();
            const auto& [off, poff, rel] = pos_info;
            const sf::Vector2f pos =
            {
                off.x + poff.x * ps.x + ps.x * rel.x - getSize().x * rel.x,
                off.y + poff.y * ps.y + ps.y * rel.y - getSize().y * rel.y
            };

            return pos;
        }
        else
        {
            const auto& [side, offset, relative_to] = anchor_pos_info;

            sf::Vector2f pos = anchor->getPosition() + offset;

            switch (side)
            {
            case Anchor_position_info::TOP:
            case Anchor_position_info::BOTTOM:
                pos.x += anchor->getSize().x * relative_to - getSize().x * relative_to;
                break;
            case Anchor_position_info::LEFT:
            case Anchor_position_info::RIGHT:
                pos.y += anchor->getSize().y * relative_to - getSize().y * relative_to;
            }

            switch (side)
            {
            case Anchor_position_info::TOP:
                pos.y -= getSize().y;
                break;
            case Anchor_position_info::BOTTOM:
                pos.y += anchor->getSize().y;
                break;
            case Anchor_position_info::LEFT:
                pos.x -= getSize().x;
                break;
            case Anchor_position_info::RIGHT:
                pos.x += anchor->getSize().x;
            }

            return pos;
        }
    }

    sf::Vector2f Gui_element::getGlobalPosition() const
    {
        return getParentPosition() + getPosition();
    }

    void Gui_element::setPositionInfo(Position_info p_info)
    {
        pos_info = p_info;
    }

    Position_info Gui_element::getPositionInfo() const
    {
        return pos_info;
    }

    void Gui_element::setSizeInfo(Size_info s_info)
    {
        size_info = s_info;
    }

    Size_info Gui_element::getSizeInfo() const
    {
        return size_info;
    }

    sf::Vector2i Gui_element::getSize() const
    {
        const auto& p = size_info.percentage;
        const auto  s = getParentSize();
        return { static_cast<int>(std::round(size_info.fixed.x + s.x * p.x)),
                 static_cast<int>(std::round(size_info.fixed.y + s.y * p.y)) };
    }

    void Gui_element::setAnchor(const Gui_element* a)
    {
        anchor = a;
    }

    const Gui_element* Gui_element::getAnchor() const
    {
        return anchor;
    }

    void Gui_element::setAnchorPositionInfo(Anchor_position_info a_info)
    {
        anchor_pos_info = a_info;
    }

    Anchor_position_info Gui_element::getAnchorPositionInfo() const
    {
        return anchor_pos_info;
    }
    void Gui_element::setParent(const Gui_element* p)
    {
        parent = p;
    }
    const Gui_element* Gui_element::getParent() const
    {
        return parent;
    }
}
