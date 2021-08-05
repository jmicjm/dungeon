#include "gui_element.h"
#include "SFML/Window/Mouse.hpp"
#include "SFML/Graphics/Sprite.hpp"


namespace gui
{
    void Gui_element::draw(sf::Drawable& drawable, const sf::RenderStates& states)
    {
        rtex.draw(drawable, states);
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

        const sf::Vector2i size_diff = sf::Vector2i{ getSize() } - sf::Vector2i{ rtex.getSize() };
        if (size_diff != sf::Vector2i{0,0})
        {
            rtex.create(getSize().x, getSize().y);
        }
        if (isRedrawRequired() || size_diff != sf::Vector2i{0,0})
        {
            rtex.clear({ 0,0,0,0 });
            redraw(size_diff);
            rtex.display();
        }

        sf::Sprite elem_sprite(rtex.getTexture());
        elem_sprite.setPosition(getPosition());
        window.draw(elem_sprite);

        window.setView(old_view);
    }

    bool Gui_element::isHovered() const
    {
        const sf::Vector2i mp = sf::Mouse::getPosition(window);
        const sf::Vector2f tl = getPosition();
        const sf::Vector2f br = tl + getSize();
        return mp.x >= tl.x && mp.x <= br.x 
            && mp.y >= tl.y && mp.y <= br.y;
    }

    sf::Vector2f Gui_element::getPosition() const
    {
        if (!anchor)
        {
            const sf::Vector2f window_size = sf::Vector2f{ window.getSize() };
            const auto& [off, rel] = pos_info;
            const sf::Vector2f pos =
            {
                off.x + window_size.x * rel.x / 100 - getSize().x * rel.x / 100,
                off.y + window_size.y * rel.y / 100 - getSize().y * rel.y / 100
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
                pos.x += anchor->getSize().x * (relative_to / 100.f) - getSize().x * (relative_to / 100.f);
                break;
            case Anchor_position_info::LEFT:
            case Anchor_position_info::RIGHT:
                pos.y += anchor->getSize().y * (relative_to / 100.f) - getSize().y * (relative_to / 100.f);
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

    void Gui_element::setPositionInfo(const Position_info& p_info)
    {
        pos_info = p_info;
    }

    Position_info Gui_element::getPositionInfo() const
    {
        return pos_info;
    }

    void Gui_element::setSizeInfo(const Size_info& s_info)
    {
        size_info = s_info;
    }

    Size_info Gui_element::getSizeInfo() const
    {
        return size_info;
    }

    sf::Vector2f Gui_element::getSize() const
    {
        const auto& p = size_info.percentage;
        const auto  w = window.getSize();
        return size_info.fixed + sf::Vector2f{w.x*p.x/100, w.y*p.y/100};
    }

    void Gui_element::setAnchor(std::shared_ptr<Gui_element> a)
    {
        anchor = a;
    }

    std::shared_ptr<Gui_element> Gui_element::getAnchor() const
    {
        return anchor;
    }

    void Gui_element::setAnchorPositionInfo(const Anchor_position_info& a_info)
    {
        anchor_pos_info = a_info;
    }

    Anchor_position_info Gui_element::getAnchorPositionInfo() const
    {
        return anchor_pos_info;
    }
}
