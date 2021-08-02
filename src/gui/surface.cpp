#include "surface.h"

namespace gui
{
    void resizeSurface(surface& surf, const sf::Vector2f& size)
    {
        sf::RectangleShape* rs = std::get_if<sf::RectangleShape>(&surf);
        if (rs != nullptr)
        {
            rs->setSize(size);
            return;
        }

        auto resizeSpriteT = [&](auto& sprite)
        {
            const sf::Vector2f spr_size =
            {
                sprite.getGlobalBounds().width,
                sprite.getGlobalBounds().height
            };
            sprite.scale(size.x / spr_size.x, size.y / spr_size.y);
        };

        sf::Sprite* spr = std::get_if<sf::Sprite>(&surf);
        if (spr != nullptr)
        {
            resizeSpriteT(*spr);
            return;
        }

        Animated_sprite* aspr = std::get_if<Animated_sprite>(&surf);
        if (aspr != nullptr)
        {
            resizeSpriteT(*aspr);
        }
    }

    void positionSurface(surface& surf, const sf::Vector2f& position)
    {
        sf::RectangleShape* rs = std::get_if<sf::RectangleShape>(&surf);
        if (rs != nullptr)
        {
            rs->setPosition(position);
            return;
        }

        sf::Sprite* spr = std::get_if<sf::Sprite>(&surf);
        if (spr != nullptr)
        {
            spr->setPosition(position);
            return;
        }

        Animated_sprite* aspr = std::get_if<Animated_sprite>(&surf);
        if (aspr != nullptr)
        {
            aspr->setPosition(position);
        }
    }

    void drawSurface(surface& surf, sf::RenderTarget& rt)
    {
        sf::RectangleShape* rs = std::get_if<sf::RectangleShape>(&surf);
        if (rs != nullptr)
        {
            rt.draw(*rs);
            return;
        }

        sf::Sprite* spr = std::get_if<sf::Sprite>(&surf);
        if (spr != nullptr)
        {
            rt.draw(*spr);
            return;
        }

        Animated_sprite* aspr = std::get_if<Animated_sprite>(&surf);
        if (aspr != nullptr)
        {
            aspr->updateFrameIdx();
            rt.draw(*aspr);
        }
    }
}
