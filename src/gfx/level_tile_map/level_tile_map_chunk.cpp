#include "level_tile_map_chunk.h"
#include "../primitive_sprite.h"
#include "../../asset_storage/tile_sprite_storage.h"


void Level_tile_map_chunk::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    st.texture = &texture->getTexture();
    rt.draw(vertices, st);
}

void Level_tile_map_chunk::copyToBuffer(const sf::VertexArray& vertex_array)
{
    vertices.setPrimitiveType(sf::Quads);
    vertices.create(vertex_array.getVertexCount());
    vertices.update(&(vertex_array[0]));
}

Level_tile_map_chunk::Level_tile_map_chunk(const Level_structure& ls, const sf::Vector2f& tile_size, const sf::IntRect& area, Combined_texture* texture)
{
    this->texture = texture;
    sf::VertexArray vertex_array;

    for (int x = area.left; x < ls.getSize().x && x < area.left + area.width; x++)
    {
        for (int y = area.top; y < ls.getSize().y && y < area.top + area.height; y++)
        {
            for (const auto& sprite_data : ls.at({ x,y }).sprites_info)
            {
                auto spr = Tile_sprite_storage::getSprite(sprite_data.id)->at(sprite_data.id_variant);

                const sf::Vector2f tex_offset = texture->getTextureOffset(spr.sprite.texture);
                spr.sprite.moveTex(tex_offset);
                spr.sprite.move({ tile_size.x * x, tile_size.y * y });	

                for (const auto& vertex : spr.sprite.vertices)
                {
                    vertex_array.append(vertex);
                }
            }
        }
    }
    copyToBuffer(vertex_array);
}

void Level_tile_map_chunk::setTexture(Combined_texture* tex)
{
    texture = tex;
}