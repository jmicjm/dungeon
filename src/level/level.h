#pragma once
#include "level_structure.h"
#include "../gfx/level_tile_map/level_tile_map.h"
#include "tile_reveal_mask.h"
#include "../gfx/view_range_overlay.h"
#include "level_params.h"
#include "door_controller.h"
#include "../entities/entity.fwd.h"
#include "../utils/quadtree.h"

#include "SFML/Graphics/Drawable.hpp"

#include <memory>

class Level : public sf::Drawable
{
    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
public:
    Level_structure structure;
    Level_tile_map tile_map;
    sf::Vector2i tile_size;
    Tile_reveal_mask reveal_mask;
    View_range_overlay view_range_overlay;

    Quadtree<std::shared_ptr<Entity>> entities;
    Door_controller door_controller{this};


    void create(const Level_params& params);
};