#pragma once
#include "world_params.h"
#include "../level/level.fwd.h"
#include "../entities/player.h"
#include "../utils/quadtree.h"
#include "../gfx/view_follower.h"

#include "SFML/Graphics/Drawable.hpp"

#include <entt/entt.hpp>

#include <memory>
#include <unordered_map>
#include <unordered_set>


class World : public sf::Drawable
{
    std::unordered_set<std::shared_ptr<Level>> levels;
    std::shared_ptr<Level> current_level;
    View_follower vf;
    View_follower vf_instant;

    entt::registry registry;
    std::unordered_map<const Level*, Quadtree<entt::entity>> entity_level_map;
    entt::entity player;

    void addLevel(std::shared_ptr<Level> level);
    void removeLevel(const std::shared_ptr<Level>& level);

    void createPlayer();
    void initViewFollowers();

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    World(const World_params& params);

    void update(sf::RenderTarget& rt);
    std::shared_ptr<Level> changeLevel(std::shared_ptr<Level> level);

    entt::entity getPlayer() const;
    const entt::registry& getRegistry() const;
};