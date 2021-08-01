#include "gui_element.h"
#include "SFML/Window/Mouse.hpp"


namespace gui
{
    Gui_element::Gui_element(sf::RenderWindow& rw)
        : window(rw) {}

    void Gui_element::draw(bool u)
    {
        if (u) update();

        const sf::View old_view = window.getView();
        const sf::View view{ sf::FloatRect{ 0,0, window.getSize().x * 1.f, window.getSize().y * 1.f } };
        window.setView(view);

        drawAction();

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
        const sf::Vector2f window_size = sf::Vector2f{ window.getSize() };
        const auto& [off, rel] = pos_info;
        const sf::Vector2f pos =
        {
            off.x + window_size.x * rel.x / 100 - getSize().x * rel.x / 100,
            off.y + window_size.y * rel.y / 100 - getSize().y * rel.y / 100
        };

        return pos;
    }

    void Gui_element::setPositionInfo(const Position_info& p_info)
    {
        pos_info = p_info;
    }

    Position_info gui::Gui_element::getPositionInfo() const
    {
        return pos_info;
    }

    void Gui_element::setSize(const sf::Vector2f& s)
    {
        size = s;
    }

    sf::Vector2f Gui_element::getSize() const
    {
        return size;
    }
}