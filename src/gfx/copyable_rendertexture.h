#pragma once
#include "SFML/Graphics/RenderTexture.hpp"

class Copyable_rendertexture : public sf::RenderTexture
{
    void copy(const Copyable_rendertexture& other);
public:
    Copyable_rendertexture() {}
    Copyable_rendertexture(const Copyable_rendertexture& other);
    Copyable_rendertexture(Copyable_rendertexture&& other) noexcept = default;
    Copyable_rendertexture& operator=(const Copyable_rendertexture& other);
    Copyable_rendertexture& operator=(Copyable_rendertexture&& other) noexcept = default;
};