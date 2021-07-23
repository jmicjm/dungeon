#include "mergeTexture.h"
#include "../primitive_sprite.h"

#include "SFML/Graphics/RenderTexture.hpp"

#include <algorithm>

sf::Vector2f mergeTexture(sf::Texture& a, const sf::Texture& b)
{
    Primitive_sprite a_sprite(&a, { {0,0}, sf::Vector2i{a.getSize()} });
    Primitive_sprite b_sprite(&b, { {0,0}, sf::Vector2i{b.getSize()} });
    sf::Vector2f b_offset = { 0, (float)a.getSize().y };
    b_sprite.move(b_offset);

    sf::Vector2u size =
    {
        std::max(a.getSize().x, b.getSize().x),
        a.getSize().y + b.getSize().y
    };

    sf::RenderTexture rtex;
    rtex.create(size.x, size.y);
    rtex.clear({0,0,0,0});
    rtex.draw(a_sprite.vertices, 4, sf::Quads, &a);
    rtex.draw(b_sprite.vertices, 4, sf::Quads, &b);
    rtex.display();

    a = rtex.getTexture();

    return b_offset;
}