#pragma once
#include "level_structure.h"
#include "../gfx/level_tile_map/level_tile_map.h"
#include "tile_reveal_mask.h"
#include "../gfx/view_range_overlay.h"
#include "level_params.h"
#include "../utils/quadtree.h"
#include "../world/world.fwd.h"
#include "../utils/sf_vector2_hash.h"

#include "SFML/Graphics/Drawable.hpp"

#include <entt/entt.hpp>

#include <memory>


class Level : public sf::Drawable
{
    Level_structure structure;
    Tile_reveal_mask reveal_mask;
    std::unique_ptr<Level_tile_map> tile_map;
    std::unique_ptr<View_range_overlay> view_range_overlay;
    std::unordered_map<sf::Vector2i, Tile_visibility_info> visible_tiles;

    std::unordered_map<const Level*, Quadtree<entt::entity>>& entity_level_map;
    entt::registry& registry;

    void placeDoors();

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    Level(const Level_params& params, entt::registry& registry, std::unordered_map<const Level*, Quadtree<entt::entity>>& entity_level_map);
    ~Level();

    const Level_structure& getStructure() const;
    const Quadtree<entt::entity>& getEntities() const;
    const entt::registry& getRegistry() const;
    void update(sf::RenderTarget& rt);
    void updateVisibleTiles(const std::unordered_map<sf::Vector2i, Tile_visibility_info>& visible_tiles, const sf::RenderTarget& rt);
    auto getVisibleTiles() const -> const decltype(visible_tiles)&;

    bool isPassable(const sf::Vector2i& position) const;
    bool isOpaque(const sf::Vector2i& position) const;

    void loadVisuals();
    void unloadVisuals();
};
