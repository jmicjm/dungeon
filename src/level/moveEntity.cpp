#include "moveEntity.h"
#include "../entities/gate.h"
#include "../components/render_component.h"
#include "../components/pending_animation.h"
#include "../components/move_animation_id.h"
#include "../asset_storage/entity_animation_storage.h"
#include "../gfx/zlevels.h"


void moveEntity(entt::registry& registry, entt::entity entity, sf::Vector2i offset)
{
    auto position = registry.try_get<Position>(entity);
    if (!position) return;

    const auto old_coords = position->getCoords();
    const auto& level = *position->getLevel();
    const auto& entities = position->getLevel()->getEntities();

    offset.x = std::clamp(offset.x, -1, 1);
    offset.y = std::clamp(offset.y, -1, 1);

    const sf::Vector2i new_coords = old_coords + offset;
    if (!entities.getArea().contains(new_coords)) return;

    if (new_coords == old_coords) return;

    auto moveToNewCoords = [&]
    {
        position->setCoords(new_coords);

        entities.forEach(old_coords, [&](auto& e) {
            if (auto gate = registry.try_get<Gate>(e.second)) registry.emplace_or_replace<Gate_moving_from>(e.second, entity);
        });

        if (auto anim_id = registry.try_get<Move_animation_id>(entity))
        {
            if (auto rc = registry.try_get<Render_component>(entity))
            {
                if (auto pa = registry.try_get<Pending_animation>(entity))
                {
                    pa->restore(*rc);
                }

                auto [zlevel, anim] = Entity_animation_storage::getMoveAnimation(anim_id->id, offset);

                auto newRc = [=](Render_component curr_rc)
                {
                    auto setReplacementZlevel = [&](auto& map)
                    {
                        if (map.find(zlevel) == map.end()) map.insert({ zlevel, {} });
                    };
                    setReplacementZlevel(curr_rc.zlevel_animation_map);
                    setReplacementZlevel(curr_rc.shadow_zlevel_animation_map);

                    return curr_rc;
                };

                registry.emplace_or_replace<Pending_animation>(
                    entity,
                    Pending_animation
                    {
                        .tracked_zlevel = zlevel,
                        .new_zlevels = newRc(*rc),
                        .src = old_coords,
                        .dst = new_coords
                    }
                );

                anim.restart();
                rc->zlevel_animation_map[zlevel] = { { anim } };
                rc->shadow_zlevel_animation_map[zlevel] = { { anim } };
            }
        }
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