#pragma once
#include "../level/level_structure.h"
#include "../utils/tile_visibility.h"
#include "../level/tile_sprite_id_enum.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTexture.hpp"

#include <map>

class View_range : public sf::Drawable
{
    sf::RenderTexture view_range_overlay_tex;
    std::vector<Tile_visibility_info> last_visible_tiles;
    sf::View last_display_view;

    std::map<TILE_SPRITE_ID::tile_sprite_id_t, sf::Sprite> sprites;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    View_range();
    void update(const Level_structure& ls, const std::vector<Tile_visibility_info>& visible_tiles, sf::RenderTarget& rt);
};