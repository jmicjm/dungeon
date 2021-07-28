#include "view_range_overlay.h"
#include "../asset_storage/texture_bank.h"

#include "SFML/Graphics/RectangleShape.hpp"

#include <utility>
#include <string>
#include <algorithm>

using namespace TILE_SPRITE_ID;

void View_range_overlay::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    const sf::View current_view = rt.getView();
    rt.setView(sf::View{ sf::FloatRect{{0.f,0.f}, sf::Vector2f{rt.getSize()}} });
    rt.draw(sf::Sprite{ overlay_tex.getTexture() });
    rt.setView(current_view);
}

View_range_overlay::View_range_overlay()
{
    const std::pair<TILE_SPRITE_ID::tile_sprite_id_t, std::string> ids[] =
    {
        {TL|TR|BL|BR, "tile_tl_tr_bl_br.png"},
        {TL|TR|BL,    "wall_tl_tr_bl.png"   },
        {TL|TR|BR,    "wall_tl_tr_br.png"   },
        {TL|BL|BR,    "wall_tl_bl_br.png"   },
        {TR|BL|BR,    "wall_tr_bl_br.png"   },
        {TL|TR,       "wall_tl_tr.png"      },
        {BL|BR,       "wall_bl_br.png"      },
        {TL|BL,       "wall_tl_bl.png"      },
        {TR|BR,       "wall_tr_br.png"      },
        {TL,          "wall_tl.png"         },
        {TR,          "wall_tr.png"         },
        {BL,          "wall_bl.png"         },
        {BR,          "wall_br.png"         }
    }; 

    for (const auto& id : ids)
    {
        sf::Sprite spr{ *Texture_bank::getTexture(id.second) };
        spr.setOrigin(8, 8);
        sprites.insert({ id.first, spr });
    }
}

void View_range_overlay::update(const Level_structure& ls, 
                                const std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>& visible_tiles,
                                const Tile_reveal_mask& revealed_tiles, 
                                const sf::RenderTarget& rt)
{
    if (   visible_tiles != last_visible_tiles
        || rt.getView().getTransform() != last_display_view.getTransform()
        || rt.getView().getSize() != last_display_view.getSize()
        )
    {
        overlay_tex.create(rt.getSize().x, rt.getSize().y);
        overlay_tex.clear({ 0,0,0,255 });
        overlay_tex.setView(rt.getView());


        const sf::Vector2f tl_px_visible = rt.getView().getCenter() - rt.getView().getSize() / 2.f;
        const sf::Vector2f br_px_visible = rt.getView().getCenter() + rt.getView().getSize() / 2.f;
        const sf::Vector2i tl_tile_visible = 
        {
            std::max(0, static_cast<int>(tl_px_visible.x) / 64),
            std::max(0, static_cast<int>(tl_px_visible.y) / 64)
        };
        const sf::Vector2i br_tile_visible =
        {
            std::min(ls.getSize().x-1, static_cast<int>(br_px_visible.x) / 64),
            std::min(ls.getSize().y-1, static_cast<int>(br_px_visible.y) / 64)
        };

        for (int x = tl_tile_visible.x; x<= br_tile_visible.x; x++)
        {
            for (int y = tl_tile_visible.y; y <= br_tile_visible.y; y++)
            {
                if (ls.isPositionValid({ x,y }))
                {
                    if (revealed_tiles.at({ x,y }).isVisible())
                    {
                        drawTileOverlay({ x,y }, ls.at({ x,y }), revealed_tiles.at({ x,y }));
                    }
                }
            }
        }

        sf::RectangleShape dark_overlay(sf::Vector2f{ overlay_tex.getSize() });
        dark_overlay.setFillColor({ 0,0,0,224 });

        overlay_tex.setView(sf::View{ sf::FloatRect{{0,0}, sf::Vector2f{overlay_tex.getSize()} } });
        overlay_tex.draw(dark_overlay);
        overlay_tex.setView(rt.getView());

        for (const auto& tile : visible_tiles)
        {
            const auto& [position, tvi] = tile;
            if (ls.isPositionValid({ position.x,position.y }))
            {
                drawTileOverlay(position, ls.at({ position.x, position.y }), tvi);
            }
        }

        overlay_tex.display();
    }
    last_visible_tiles = visible_tiles;
    last_display_view = rt.getView();
}

void View_range_overlay::drawTileOverlay(const sf::Vector2i& position, const Tile& tile, Tile_visibility_info tvi)
{
    const tile_sprite_id_t id = [&]() {
        const auto& [tl, tr, bl, br] = tvi;
        if (tile.type != TILE_TYPE::WALL)
        {
            return TL | TR | BL | BR;
        }
        return tl * TL | tr * TR | bl * BL | br * BR;
    }();

    auto it = sprites.find(id);
    if (it != sprites.end())
    {
        it->second.setPosition(sf::Vector2f{ position }*64.f);
        overlay_tex.draw(it->second, sf::BlendMultiply);
    }
}