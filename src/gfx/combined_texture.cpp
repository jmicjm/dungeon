#include "combined_texture.h"
#include "utils/mergeTexture.h"


sf::Vector2f Combined_texture::getTextureOffset(const sf::Texture* texture)
{
    const auto it = texture_offset_map.find(texture);
    if (it != texture_offset_map.end())
    {
        return it->second;
    }

    const sf::Vector2f tex_offset = mergeTexture(combined_texture, *texture);
    texture_offset_map.insert({ texture, tex_offset });

    return tex_offset;
}

const sf::Texture& Combined_texture::getTexture() const
{
    return combined_texture;
}