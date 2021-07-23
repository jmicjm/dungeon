#include "level_tile_map_chunk.h"
#include "../primitive_sprite.h"
#include "../../asset_storage/tile_sprite_storage.h"


void Level_tile_map_chunk::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    st.texture = &texture->getCombinedTexture();
    rt.draw(vertices, st);
}

void Level_tile_map_chunk::copyToBuffer(const sf::VertexArray& vertex_array)
{
    vertices.setPrimitiveType(sf::Quads);
    vertices.create(vertex_array.getVertexCount());
    vertices.update(&(vertex_array[0]));
}

void Level_tile_map_chunk::populate(const Level_structure& ls, const sf::Vector2f& tile_size, const sf::IntRect& area)
{
    sf::VertexArray vertex_array;

    for (int x = area.left; x < ls.getSize().x && x < area.left + area.width; x++)
    {
        for (int y = area.top; y < ls.getSize().y && y < area.top + area.height; y++)
        {
            for (const auto& sprite_data : ls.at({ x,y }).sprites_info)
            {
                Primitive_sprite spr = Tile_sprite_storage::getSprite(sprite_data.id)->at(sprite_data.id_variant);

                const sf::Vector2f tex_offset = texture->getTextureOffset(spr.texture);
                spr.moveTex(tex_offset);
                spr.move({ tile_size.x * x, tile_size.y * y });	

                for (const auto& vertex : spr.vertices)
                {
                    vertex_array.append(vertex);
                }
            }
        }
    }
    copyToBuffer(vertex_array);
}

void Level_tile_map_chunk::setTexture(Level_tile_map_texture* tex)
{
    texture = tex;
}