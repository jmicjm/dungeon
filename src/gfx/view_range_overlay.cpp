#include "view_range_overlay.h"
#include "../level/level.h"
#include "../asset_storage/texture_bank.h"
#include "../utils/sf_vector2_utils.h"
#include "utils/visibleAreaBounds.h"

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

View_range_overlay::View_range_overlay()
{
    const std::pair<TILE_SPRITE_ID::tile_sprite_id_t, std::string> ids[] =
    {
        {TL|TR|BL|BR, "assets/tiles/view_range_overlay/tile_tl_tr_bl_br.png"},
        {TL|TR|BL,    "assets/tiles/view_range_overlay/wall_tl_tr_bl.png"   },
        {TL|TR|BR,    "assets/tiles/view_range_overlay/wall_tl_tr_br.png"   },
        {TL|BL|BR,    "assets/tiles/view_range_overlay/wall_tl_bl_br.png"   },
        {TR|BL|BR,    "assets/tiles/view_range_overlay/wall_tr_bl_br.png"   },
        {TL|TR,       "assets/tiles/view_range_overlay/wall_tl_tr.png"      },
        {BL|BR,       "assets/tiles/view_range_overlay/wall_bl_br.png"      },
        {TL|BL,       "assets/tiles/view_range_overlay/wall_tl_bl.png"      },
        {TR|BR,       "assets/tiles/view_range_overlay/wall_tr_br.png"      },
        {TL,          "assets/tiles/view_range_overlay/wall_tl.png"         },
        {TR,          "assets/tiles/view_range_overlay/wall_tr.png"         },
        {BL,          "assets/tiles/view_range_overlay/wall_bl.png"         },
        {BR,          "assets/tiles/view_range_overlay/wall_br.png"         }
    }; 

    for (const auto& id : ids)
    {
        sf::Sprite spr{ *Texture_bank::getTexture(id.second) };
        spr.setOrigin(8, 8);
        sprites.insert({ id.first, spr });
    }
}

void View_range_overlay::update(const Level& l, 
                                const std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>& visible_tiles,
                                const Tile_reveal_mask& revealed_tiles, 
                                const sf::RenderTarget& rt)
{
    if (   visible_tiles != last_visible_tiles
        || rt.getView().getTransform() != last_display_view.getTransform()
        || rt.getView().getSize() != last_display_view.getSize())
    {
        overlay_tex.create(rt.getSize().x, rt.getSize().y);
        overlay_tex.clear({ 0,0,0,255 });
        overlay_tex.setView(rt.getView());

        const auto [tl_tile, br_tile] = [&]()
        {
            const auto bounds = visibleAreaBounds(rt.getView(), l.tile_size);
            const sf::Vector2i tl_tile = { std::max(0, bounds.first.x),
                                           std::max(0, bounds.first.y) };
            const sf::Vector2i br_tile = { std::min(l.structure.getSize().x-1, bounds.second.x),
                                           std::min(l.structure.getSize().y-1, bounds.second.y) };
            return std::pair{ tl_tile, br_tile };
        }();

        for (int x = tl_tile.x; x <= br_tile.x; x++)
        {
            for (int y = tl_tile.y; y <= br_tile.y; y++)
            {
                if (revealed_tiles.at({ x,y }).isVisible())
                {
                    drawTileOverlay(l, { x,y }, revealed_tiles.at({ x,y }));
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
            if (l.structure.isPositionValid(position))
            {
                drawTileOverlay(l, position, tvi);
            }
        }

        overlay_tex.display();
    }
    last_visible_tiles = visible_tiles;
    last_display_view = rt.getView();
}

void View_range_overlay::drawTileOverlay(const Level& l, 
                                         const sf::Vector2i& position,
                                         const Tile_visibility_info tvi)
{
    const tile_sprite_id_t id = [&]() 
    {
        const auto& [tl, tr, bl, br] = tvi;
        const bool is_closed_door = [&]()
        {
            auto doors = l.door_controller.doors.find(position);
            return doors.size() > 0 && doors[0]->second.state == Door::CLOSED;
        }();

        if (l.structure.at(position).type == TILE_TYPE::WALL || is_closed_door)
        {
            return tl * TL | tr * TR | bl * BL | br * BR;
        }
        return TL | TR | BL | BR;       
    }();

    if (auto it = sprites.find(id); it != sprites.end())
    {
        it->second.setPosition(sf::Vector2f{ vecMul(position, l.tile_size) });
        overlay_tex.draw(it->second, sf::BlendMultiply);
    }
}