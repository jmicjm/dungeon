#include "view_range_overlay.h"
#include "../level/level.h"
#include "../asset_storage/texture_bank.h"
#include "../utils/sf_vector2_utils.h"
#include "../asset_storage/tile_sprite_storage.h"
#include "utils/visibleAreaBounds.h"
#include "../components/opaque.h"
#include "../components/door.h"

#include "SFML/Graphics/RectangleShape.hpp"

#include <utility>
#include <string>
#include <algorithm>

using namespace TILE_SPRITE_ID;

void View_range_overlay::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    const sf::View current_view = rt.getView();
    rt.setView(sf::View{ sf::FloatRect{ { 0.f,0.f }, sf::Vector2f{ rt.getSize() } } });
    rt.draw(sf::Sprite{ overlay_tex.getTexture() }, &blur_shader);
    rt.setView(current_view);
}

View_range_overlay::View_range_overlay()
{
    const std::pair<TILE_SPRITE_ID::tile_sprite_id_t, std::string> ids[] =
    {
        {TL|TR|BL|BR, "tiles/view_range_overlay/tile_tl_tr_bl_br.png"},
        {TL|TR|BL,    "tiles/view_range_overlay/wall_tl_tr_bl.png"   },
        {TL|TR|BR,    "tiles/view_range_overlay/wall_tl_tr_br.png"   },
        {TL|BL|BR,    "tiles/view_range_overlay/wall_tl_bl_br.png"   },
        {TR|BL|BR,    "tiles/view_range_overlay/wall_tr_bl_br.png"   },
        {TL|TR,       "tiles/view_range_overlay/wall_tl_tr.png"      },
        {BL|BR,       "tiles/view_range_overlay/wall_bl_br.png"      },
        {TL|BL,       "tiles/view_range_overlay/wall_tl_bl.png"      },
        {TR|BR,       "tiles/view_range_overlay/wall_tr_br.png"      },
        {TL,          "tiles/view_range_overlay/wall_tl.png"         },
        {TR,          "tiles/view_range_overlay/wall_tr.png"         },
        {BL,          "tiles/view_range_overlay/wall_bl.png"         },
        {BR,          "tiles/view_range_overlay/wall_br.png"         }
    }; 

    for (const auto& id : ids)
    {
        sf::Sprite spr{ *Texture_bank::getTexture(id.second), sf::IntRect{8,8,64,64} };
        spr.setOrigin(0, 0);
        sprites.insert({ id.first, spr });
    }

    const char* shader = R"(
        uniform sampler2D texture;
        uniform vec2 texture_size;
        uniform int radius;
        uniform int quality;

        void main()
        {
            vec2 inv_texture_size = vec2(1.0 / texture_size.x, 1.0 / texture_size.y);

            vec4 color = vec4(0, 0, 0, 0);
            for (int x = -radius; x < radius; x += quality)
            {
                for (int y = -radius; y < radius; y += quality)
                {
                    color += texture2D(texture, gl_TexCoord[0].xy + vec2(x, y) * inv_texture_size);
                }
            }
            int sum = (2 * radius / quality) * (2 * radius / quality);
            gl_FragColor = gl_Color * (color / float(sum));
       }
    )";
    blur_shader.loadFromMemory(shader, sf::Shader::Fragment);
    blur_shader.setUniform("texture", sf::Shader::CurrentTexture);
    blur_shader.setUniform("quality", 1);
}

void View_range_overlay::update(const Level& l, 
                                const std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>& visible_tiles,
                                const Tile_reveal_mask& revealed_tiles, 
                                const sf::RenderTarget& rt)
{
    {
        if(overlay_tex.getSize() != rt.getSize()) overlay_tex.create(rt.getSize().x, rt.getSize().y);
        overlay_tex.clear({ 0,0,0,255 });
        overlay_tex.setView(rt.getView());

        blur_shader.setUniform("texture_size", sf::Vector2f{ overlay_tex.getSize() });
        const float radius_mul = std::min(1.f, rt.getSize().x / rt.getView().getSize().x);
        blur_shader.setUniform("radius", static_cast<int>(std::max(1.f, 8*radius_mul)));

        const auto [tl_tile, br_tile] = [&]()
        {
            const auto bounds = visibleAreaBoundsTiles(rt.getView());
            const sf::Vector2i tl_tile = { std::max(0, bounds.first.x),
                                           std::max(0, bounds.first.y) };
            const sf::Vector2i br_tile = { std::min(l.getStructure().getSize().x - 1, bounds.second.x),
                                           std::min(l.getStructure().getSize().y-1, bounds.second.y) };
            return std::pair{ tl_tile, br_tile };
        }();

        for (int x = tl_tile.x; x <= br_tile.x; x++)
        {
            for (int y = tl_tile.y; y <= br_tile.y; y++)
            {
                if (revealed_tiles.at({ x,y }).isVisible())
                {
                    drawTileOverlay(l, { x,y }, revealed_tiles.at({ x,y }), false);
                }
            }
        }

        sf::RectangleShape dark_overlay(sf::Vector2f{ overlay_tex.getSize() });
        dark_overlay.setFillColor({ 0,0,0,224 });

        overlay_tex.setView(sf::View{ sf::FloatRect{ { 0,0 }, sf::Vector2f{ overlay_tex.getSize() } } });
        overlay_tex.draw(dark_overlay);
        overlay_tex.setView(rt.getView());

        for (const auto& tile : visible_tiles)
        {
            const auto& [position, tvi] = tile;
            if (l.getStructure().isPositionValid(position))
            {
                drawTileOverlay(l, position, tvi, true);
            }
        }

        overlay_tex.display();
    }
}

void View_range_overlay::drawTileOverlay(const Level& l, 
                                         const sf::Vector2i& position,
                                         const Tile_visibility_info tvi,
                                         const bool visible)
{
    const tile_sprite_id_t id = [&]
    {
        const auto& [tl, tr, bl, br] = tvi;
        if (l.getStructure().at(position).type == TILE_TYPE::WALL 
            || l.getEntities().forEachUntil(position, [&](auto& entity) { return visible ? l.getRegistry().all_of<Door, Opaque>(entity.second) : l.getRegistry().all_of<Door>(entity.second); }))
        {
            return tl * TL | tr * TR | bl * BL | br * BR;
        }
        return TL | TR | BL | BR;       
    }();

    if (auto it = sprites.find(id); it != sprites.end())
    {
        it->second.setPosition(sf::Vector2f{ vecMul(position, Tile_sprite_storage::tile_size) });
        overlay_tex.draw(it->second, sf::BlendMultiply);
    }
}