#include "moveEntity.h"
#include "../entities/gate.h"


void moveEntity(entt::registry& registry, Quadtree<entt::entity>& entities, Position& position, sf::Vector2i offset)
{
    const auto old_coords = position.getCoords();
    const auto& level = *position.getLevel();

    offset.x = std::clamp(offset.x, -1, 1);
    offset.y = std::clamp(offset.y, -1, 1);

    const sf::Vector2i new_coords = old_coords + offset;
    if (!entities.getArea().contains(new_coords)) return;

    if (new_coords == old_coords) return;

    auto moveToNewCoords = [&]
    {
        position.setCoords(new_coords);

        auto tile_entities = entities.find(old_coords);
        for (auto e : tile_entities)
        {
            if (auto gate = registry.try_get<Gate>(e->second)) closeGate(registry, entities, e->second);
        }
    };

    if (!level.isPassable(new_coords))
    {
        const bool gate_open_success = [&]
        {
            auto tile_entities = entities.find(new_coords);
            for (auto e : tile_entities)
            {
                if (auto gate = registry.try_get<Gate>(e->second)) return openGate(registry, entities, e->second);
            }        
            return false;
        }();

        if (gate_open_success) moveToNewCoords();
    }
    else moveToNewCoords();
}