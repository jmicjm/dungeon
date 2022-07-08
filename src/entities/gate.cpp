#include "gate.h"
#include "../components/position.h"
#include "../components/nonpassable.h"
#include "../components/render_component.h"


bool openGate(entt::registry& registry, const Quadtree<entt::entity>& entities, entt::entity gate)
{
    auto gate_c = registry.try_get<Gate>(gate);
    if (!gate_c || gate_c->is_open) return false;

    auto coords = registry.get<Position>(gate).getCoords();
    if (entities.find(coords).size() > 1) return false;

    registry.replace<Render_component>(gate, gate_c->open_rc);
    if (!(gate_c->passability & Gate::OPEN)) registry.emplace_or_replace<Nonpassable>(gate);
    else registry.remove<Nonpassable>(gate);
    gate_c->is_open = true;

    return true;
}

bool closeGate(entt::registry& registry, const Quadtree<entt::entity>& entities, entt::entity gate)
{
    auto gate_c = registry.try_get<Gate>(gate);
    if (!gate_c || !gate_c->is_open) return false;

    auto coords = registry.get<Position>(gate).getCoords();
    if (entities.find(coords).size() > 1) return false;

    registry.replace<Render_component>(gate, gate_c->closed_rc);
    if (!(gate_c->passability & Gate::CLOSED)) registry.emplace_or_replace<Nonpassable>(gate);
    else registry.remove<Nonpassable>(gate);
    gate_c->is_open = false;

    return true;
}
