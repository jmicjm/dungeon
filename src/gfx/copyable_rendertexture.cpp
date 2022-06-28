#include "copyable_rendertexture.h"
#include "utils/setQuad.h"

void Copyable_rendertexture::copy(const Copyable_rendertexture& other)
{
    if (&other == this) return;

    create(other.getSize().x, other.getSize().y);
    sf::Vertex vertices[4];
    setQuad(vertices, { {0,0}, sf::Vector2i{other.getSize()} });

    clear({0,0,0,0});
    draw(vertices, 4, sf::Quads, &other.getTexture());
    display();

    setView(other.getView());
    setSmooth(other.isSmooth());
    setRepeated(other.isRepeated());
    //setActive(other.isActiveDoesNotExist()); //same goes for ContextSettings in .create()
}

Copyable_rendertexture::Copyable_rendertexture(const Copyable_rendertexture& other)
{
    copy(other);
}

Copyable_rendertexture& Copyable_rendertexture::operator=(const Copyable_rendertexture& other)
{
    copy(other);
    return *this;
}