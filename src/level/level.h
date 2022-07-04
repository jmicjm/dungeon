#pragma once
#include "level_structure.h"
#include "../gfx/level_tile_map/level_tile_map.h"
#include "tile_reveal_mask.h"
#include "../gfx/view_range_overlay.h"
#include "level_params.h"
#include "door_controller.h"
#include "../utils/quadtree.h"
#include "entrance.h"
#include "../world/world.fwd.h"

#include <entt/entt.hpp>

#include <memory>


class Level
{
    Level_structure structure;
    Tile_reveal_mask reveal_mask;
    std::unique_ptr<Level_tile_map> tile_map;
    std::unique_ptr<View_range_overlay> view_range_overlay;

public:  
    Quadtree<Entrance> entrances;
    Door_controller door_controller;

    Level(const Level_params& params);

    const Level_structure& getStructure() const;
    void update(entt::registry& registry, Quadtree<entt::entity>& entities, World& world, sf::RenderTarget& rt);
    void updateVisibleTiles(const std::unordered_map<sf::Vector2i, Tile_visibility_info>& visible_tiles, const sf::RenderTarget& rt);

    void loadVisuals();
    void unloadVisuals();

    void draw(const entt::registry& registry, const Quadtree<entt::entity>& entities, sf::RenderTarget& rt, sf::RenderStates st) const;
};