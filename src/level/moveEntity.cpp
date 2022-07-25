#include "moveEntity.h"
#include "../entities/gate.h"


void moveEntity(entt::registry& registry, Position& position, sf::Vector2i offset)
{
    const auto old_coords = position.getCoords();
    const auto& level = *position.getLevel();
    const auto& entities = position.getLevel()->getEntities();

    offset.x = std::clamp(offset.x, -1, 1);
    offset.y = std::clamp(offset.y, -1, 1);

    const sf::Vector2i new_coords = old_coords + offset;
    if (!entities.getArea().contains(new_coords)) return;

    if (new_coords == old_coords) return;

    auto moveToNewCoords = [&]
    {
        position.setCoords(new_coords);

        entities.forEach(old_coords, [&](auto& entity) {
            if (auto gate = registry.try_get<Gate>(entity.second)) closeGate(registry, entities, entity.second);
        });
    };

    if (!level.isPassable(new_coords))
    {
        const bool gate_open_success = [&]
        {
            int gate_c = 0;
            int open_gate_c = 0;

            entities.forEachUntil(new_coords, [&](auto& entity) {
                if (auto gate = registry.try_get<Gate>(entity.second))
                {
                    gate_c++;
                    open_gate_c += openGate(registry, entities, entity.second);
                }
                return gate_c != open_gate_c;
            });
            return gate_c && gate_c == open_gate_c;
        }();

        if (gate_open_success) moveToNewCoords();
    }
    else moveToNewCoords();
}