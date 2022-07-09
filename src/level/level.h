#pragma once
#include "level_structure.h"
#include "../gfx/level_tile_map/level_tile_map.h"
#include "tile_reveal_mask.h"
#include "../gfx/view_range_overlay.h"
#include "level_params.h"
#include "../utils/quadtree.h"
#include "../world/world.fwd.h"

#include "SFML/Graphics/Drawable.hpp"

#include <entt/entt.hpp>

#include <memory>


class Level : public sf::Drawable
{
    Level_structure structure;
    Tile_reveal_mask reveal_mask;
    std::unique_ptr<Level_tile_map> tile_map;
    std::unique_ptr<View_range_overlay> view_range_overlay;

    std::unordered_map<const Level*, Quadtree<entt::entity>>* entity_level_map = nullptr;
    entt::registry* registry = nullptr;

    void placeDoors();

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    Level(const Level_params& params, entt::registry& registry, std::unordered_map<const Level*, Quadtree<entt::entity>>& entity_level_map);
    ~Level();

    const Level_structure& getStructure() const;
    void update(entt::registry& registry, World& world, sf::RenderTarget& rt);
    void updateVisibleTiles(const std::unordered_map<sf::Vector2i, Tile_visibility_info>& visible_tiles, const sf::RenderTarget& rt);

    bool isPassable(const sf::Vector2i& position) const;

    void loadVisuals();
    void unloadVisuals();
};