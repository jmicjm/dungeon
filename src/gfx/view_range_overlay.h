#pragma once
#include "../level/level.h"
#include "../level/tile_reveal_mask.h"
#include "../level/tile_visibility_info.h"
#include "../level/tile_sprite_id_enum.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTexture.hpp"

#include <map>

class View_range_overlay : public sf::Drawable
{
    sf::RenderTexture overlay_tex;
    std::vector<std::pair<sf::Vector2i, Tile_visibility_info>> last_visible_tiles;
    sf::View last_display_view;

    std::map<TILE_SPRITE_ID::tile_sprite_id_t, sf::Sprite> sprites;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

    void drawTileOverlay(const sf::Vector2i& position, 
                         const Tile& tile, 
                         const sf::Vector2i& tile_size,
                         const Tile_visibility_info tvi);

public:
    View_range_overlay();
    void update(const Level& l,
                const std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>& visible_tiles,
                const Tile_reveal_mask& revealed_tiles,
                const sf::RenderTarget& rt);
};