#include "gate.h"
#include "../components/position.h"
#include "../components/nonpassable.h"
#include "../components/opaque.h"
#include "../components/render_component.h"
#include "../components/pending_animation.h"


bool openGate(entt::registry& registry, const Quadtree<entt::entity>& entities, entt::entity gate)
{
    auto gate_c = registry.try_get<Gate>(gate);
    if (!gate_c || gate_c->is_open) return false;

    auto coords = registry.get<Position>(gate).getCoords();
    if (entities.forEachUntil(coords, [&](auto& entity) {
        return entity.second != gate;
    })) return false;

    registry.replace<Render_component>(gate, gate_c->open_rc);
    if (!(gate_c->passable_states & Gate::OPEN)) registry.emplace_or_replace<Nonpassable>(gate);
    else registry.remove<Nonpassable>(gate);
    if (gate_c->opaque_states & Gate::OPEN) registry.emplace_or_replace<Opaque>(gate);
    else registry.remove<Opaque>(gate);
    gate_c->is_open = true;

    return true;
}

bool closeGate(entt::registry& registry, const Quadtree<entt::entity>& entities, entt::entity gate)
{
    auto gate_c = registry.try_get<Gate>(gate);
    if (!gate_c || !gate_c->is_open) return false;

    auto coords = registry.get<Position>(gate).getCoords();
    if (entities.forEachUntil(coords, [&](auto& entity) {
        return entity.second != gate;
    })) return false;

    registry.replace<Render_component>(gate, gate_c->closed_rc);
    if (!(gate_c->passable_states & Gate::CLOSED)) registry.emplace_or_replace<Nonpassable>(gate);
    else registry.remove<Nonpassable>(gate);
    if (gate_c->opaque_states & Gate::CLOSED) registry.emplace_or_replace<Opaque>(gate);
    else registry.remove<Opaque>(gate);
    gate_c->is_open = false;

    return true;
}

void updateMovingFromGates(entt::registry& registry, const Quadtree<entt::entity>& entities)
{
    for (const auto& [entity, gate, gmf, pos] : registry.view<Gate, Gate_moving_from, Position>().each())
    {
        if (   !registry.valid(gmf.moving_from_entity) 
            || !registry.try_get<Pending_animation>(gmf.moving_from_entity)
            || registry.get<Pending_animation>(gmf.moving_from_entity).src != pos.getCoords())
        {
            closeGate(registry, entities, entity);
            registry.erase<Gate_moving_from>(entity);
        }
    }
}
