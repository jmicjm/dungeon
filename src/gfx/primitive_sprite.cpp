#include "primitive_sprite.h"
#include "utils/setQuad.h"

Primitive_sprite::Primitive_sprite(const sf::Texture* texture, const sf::IntRect& rect) : texture(texture)
{
    setQuad(vertices, rect);
}

void Primitive_sprite::move(const sf::Vector2f& offset)
{
    for (auto& vertex : vertices)
    {
        vertex.position += offset;
    }
}

void Primitive_sprite::moveTex(const sf::Vector2f& offset)
{
    for (auto& vertex : vertices)
    {
        vertex.texCoords += offset;
    }
}