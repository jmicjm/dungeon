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
    rt.draw(sf::Sprite{ overlay_tex.getTexture() });
    rt.setView(current_view);
}

void View_range_overlay::render(const Level& l, const pos_tvi_id_vec& visible, const pos_tvi_id_vec& revealed, const sf::RenderTarget& rt)
{
    if (overlay_tex.getSize() != rt.getSize())
    {
        overlay_tex.create(rt.getSize().x, rt.getSize().y);
        work_tex.create(rt.getSize().x, rt.getSize().y);
    }
    overlay_tex.clear({ 0,0,0,255 });
    overlay_tex.setView(rt.getView());

    for (const auto& [pos, id] : revealed)
    {
        drawTileOverlay(pos, id);
    }


    sf::RectangleShape dark_overlay(sf::Vector2f{ overlay_tex.getSize() });
    dark_overlay.setFillColor({ 0,0,0,224 });

    overlay_tex.setView(sf::View{ sf::FloatRect{ { 0,0 }, sf::Vector2f{ overlay_tex.getSize() } } });
    overlay_tex.draw(dark_overlay);
    overlay_tex.setView(rt.getView());

    for (const auto& [pos, id] : visible)
    {
        drawTileOverlay(pos, id);
    }

    overlay_tex.display();


    const auto inv_texture_size = sf::Vector2f{ 1.f / overlay_tex.getSize().x, 1.f / overlay_tex.getSize().y };
    const float radius_mul = std::min(1.f, rt.getSize().x / rt.getView().getSize().x);
    const int radius = std::max(1.f, blur_radius * radius_mul);
    blur_shader.setUniform("radius", radius);
    blur_shader.setUniform("inv_weight_sum", 1.f / std::ceil((2.f * radius + 1) / blur_quality));


    work_tex.clear({ 0,0,0,0 });
    blur_shader.setUniform("tex_pos_idx_mul", vecMul(sf::Vector2f{ 1,0 }, inv_texture_size ));
    work_tex.draw(sf::Sprite{ overlay_tex.getTexture() }, &blur_shader);
    work_tex.display();

    overlay_tex.clear({ 0,0,0,0 });
    overlay_tex.setView(sf::View{ sf::FloatRect{ { 0,0 }, sf::Vector2f{ overlay_tex.getSize() } } });
    blur_shader.setUniform("tex_pos_idx_mul", vecMul(sf::Vector2f{ 0,1 }, inv_texture_size));
    overlay_tex.draw(sf::Sprite{ work_tex.getTexture() }, &blur_shader);
    overlay_tex.display();
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
        uniform vec2 tex_pos_idx_mul;
        uniform int radius;
        uniform int quality;
        uniform float inv_weight_sum;

        void main()
        {
            vec4 color = vec4(0, 0, 0, 0);
            for (int x = -radius; x <= radius; x += quality)
            {
                color += texture2D(texture, gl_TexCoord[0].xy + float(x) * tex_pos_idx_mul);
            }
            gl_FragColor = gl_Color * color * inv_weight_sum;
       }
    )";
    blur_shader.loadFromMemory(shader, sf::Shader::Fragment);
    blur_shader.setUniform("texture", sf::Shader::CurrentTexture);
    blur_shader.setUniform("quality", blur_quality);
}

void View_range_overlay::update(
    const Level& l, 
    const std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>& visible_tiles,
    const Tile_reveal_mask& revealed_tiles, 
    const sf::RenderTarget& rt)
{
    const auto id = [&](const sf::Vector2i& position, const Tile_visibility_info tvi, const bool visible)
    {
        const auto& [tl, tr, bl, br] = tvi;
        if (l.getStructure().at(position).type == TILE_TYPE::WALL
            || l.getEntities().forEachUntil(position, [&](auto& entity) { return visible ? l.getRegistry().all_of<Door, Opaque>(entity.second) : l.getRegistry().all_of<Door>(entity.second); }))
        {
            return tl * TL | tr * TR | bl * BL | br * BR;
        }
        return TL | TR | BL | BR;
    };

    auto visible = [&]
    {
        decltype(last_visible) visible;
        for (const auto& [pos, tvi] : visible_tiles)
        {
            if (tvi.isVisible() && l.getStructure().isPositionValid(pos)) visible.emplace_back(pos, id(pos, tvi, true));
        }
        return visible;
    }();

    auto revealed = [&]
    {
        decltype(last_revealed) revealed;

        const auto [tl_tile, br_tile] = [&]
        {
            const auto bounds = visibleAreaBoundsTiles(rt.getView());
            const sf::Vector2i tl_tile = { std::max(0, bounds.first.x),
                                           std::max(0, bounds.first.y) };
            const sf::Vector2i br_tile = { std::min(l.getStructure().getSize().x - 1, bounds.second.x),
                                           std::min(l.getStructure().getSize().y - 1, bounds.second.y) };
            return std::pair{ tl_tile, br_tile };
        }();

        for (int x = tl_tile.x; x < br_tile.x; x++)
        {
            for (int y = tl_tile.y; y < br_tile.y; y++)
            {
                const auto pos = sf::Vector2i{ x,y };
                const auto tvi = revealed_tiles.at(pos);
                if (tvi.isVisible()) revealed.emplace_back(pos, id(pos, tvi, false));
            }
        }
        return revealed;
    }();  

    if (   last_view.getCenter() != rt.getView().getCenter()
        || last_view.getSize() != rt.getView().getSize()
        || last_view.getTransform() != rt.getView().getTransform()
        || overlay_tex.getSize() != rt.getSize()
        || last_visible != visible
        || last_revealed != revealed)
    {
        render(l, visible, revealed, rt);

        last_view = rt.getView();
        last_visible = std::move(visible);
        last_revealed = std::move(revealed);
    }
}

void View_range_overlay::drawTileOverlay(const sf::Vector2i& position, const TILE_SPRITE_ID::tile_sprite_id_t id)
{
    if (auto it = sprites.find(id); it != sprites.end())
    {
        it->second.setPosition(sf::Vector2f{ vecMul(position, Tile_sprite_storage::tile_size) });
        overlay_tex.draw(it->second, sf::BlendMultiply);
    }
}