#include "view_range.h"
#include "../asset_storage/texture_bank.h"

#include <utility>
#include <string>

using namespace TILE_SPRITE_ID;

void View_range::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    const sf::View current_view = rt.getView();
    rt.setView(sf::View{ sf::FloatRect{{0.f,0.f}, sf::Vector2f{rt.getSize()}} });
    rt.draw(sf::Sprite{ view_range_overlay_tex.getTexture() });
    rt.setView(current_view);
}

View_range::View_range()
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

void View_range::update(const Level_structure& ls, const std::vector<Tile_visibility_info>& visible_tiles, sf::RenderTarget& rt)
{
    if (visible_tiles != last_visible_tiles
        || rt.getView().getTransform() != last_display_view.getTransform()
        || rt.getView().getSize() != last_display_view.getSize()
        )
    {
        view_range_overlay_tex.create(rt.getSize().x, rt.getSize().y);
        view_range_overlay_tex.clear({ 0,0,0,224 });
        view_range_overlay_tex.setView(rt.getView());

        for (const auto& tile : visible_tiles)
        {
            
            const tile_sprite_id_t id = [&]() {
                const auto& [position, tl, tr, bl, br] = tile;
                if (ls.at({ position.x, position.y }).type != TILE_TYPE::WALL)
                {
                    return TL | TR | BL | BR;
                }
                return tl * TL | tr * TR | bl * BL | br * BR;
            }(); 

            auto it = sprites.find(id);
            if (it != sprites.end())
            {
                it->second.setPosition(sf::Vector2f{ tile.position }*64.f);
                view_range_overlay_tex.draw(it->second, sf::BlendMultiply);
            }
        }
        view_range_overlay_tex.display();
    }
    last_visible_tiles = visible_tiles;
    last_display_view = rt.getView();
}