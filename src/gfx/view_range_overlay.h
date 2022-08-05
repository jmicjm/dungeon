#pragma once
#include "../level/level.fwd.h"
#include "../level/tile_reveal_mask.h"
#include "../level/tile_visibility_info.h"
#include "../level/tile_sprite_id_enum.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Shader.hpp"

#include <map>

class View_range_overlay : public sf::Drawable
{
    sf::RenderTexture overlay_tex;
    sf::RenderTexture work_tex;

    sf::Shader blur_shader;

    std::map<TILE_SPRITE_ID::tile_sprite_id_t, sf::Sprite> sprites;

    using pos_tvi_id_vec = std::vector<std::pair<sf::Vector2i, TILE_SPRITE_ID::tile_sprite_id_t>>;
    pos_tvi_id_vec last_visible;
    pos_tvi_id_vec last_revealed;
    sf::View last_view;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

    void render(
        const Level& l,
        const pos_tvi_id_vec& visible,
        const pos_tvi_id_vec& revealed,
        const sf::RenderTarget& rt
    );

    void drawTileOverlay( const sf::Vector2i& position, const TILE_SPRITE_ID::tile_sprite_id_t id);

public:
    View_range_overlay();

    void update(
        const Level& l,
        const std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>& visible_tiles,
        const Tile_reveal_mask& revealed_tiles,
        const sf::RenderTarget& rt
    );
};