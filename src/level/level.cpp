#include "level.h"
#include "../gfx/utils/visibleAreaBounds.h"
#include "../level_gen/createLevelStructure.h"
#include "../utils/sf_vector2_utils.h"
#include "../gfx/animated_sprite/animated_sprite.h"
#include "../components/position.h"
#include "../entities/player.h"
#include "visibleTiles.h"
#include "../entities/door.h"
#include "../components/nonpassable.h"
#include "../components/opaque.h"
#include "../components/render_component.h"

#include <algorithm>


void Level::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    if(tile_map) rt.draw(*tile_map, st);
    const auto [tl, br] = visibleAreaBoundsTiles(rt.getView());

    std::map<int, std::vector<std::pair<sf::Vector2i, const Animated_sprite*>>> zlevel_map;

    auto visible_entities = (*entity_level_map)[this].find({ tl, br });
    for (const auto entity : visible_entities)
    {
        const auto rc = registry->try_get<Render_component>(entity->second);
        const auto pos = registry->try_get<Position>(entity->second);
        if (rc && pos)
        {
            for (const auto& [zlevel, animations] : rc->zlevel_animation_map)
            {
                for (const auto& animation : animations)
                {
                    zlevel_map[zlevel].push_back({ pos->getCoords(), &animation });
                }
            }
        }
    }

    auto pixelPosition = [](const sf::Vector2i& tile_position)
    {
        return vecMul(sf::Vector2f{ tile_position }, Tile_sprite_storage::tile_size);
    };

    for (auto& [zlevel, animations] : zlevel_map)
    {
        std::ranges::sort(animations, [&](const auto& a, const auto& b) { return a.first.y * structure.getSize().x + a.first.x < b.first.y * structure.getSize().x + b.first.x; });
        for (const auto& [pos, animation] : animations)
        {
            sf::RenderStates st2 = st;
            st2.transform.translate(pixelPosition(pos));
            rt.draw(*animation, st2);
        }
    }
    
    if(view_range_overlay) rt.draw(*view_range_overlay);
}

void Level::placeDoors()
{
    for (int x = 1; x < structure.getSize().x - 1; x++)
    {
        for (int y = 1; y < structure.getSize().y - 1; y++)
        {
            if (structure.at({ x,y }).type == TILE_TYPE::DOORWAY)
            {
                if (structure.at({ x - 1,y }).type == TILE_TYPE::WALL && structure.at({ x + 1,y }).type == TILE_TYPE::WALL)
                {
                    auto door = createDoorFront(*registry);
                    registry->emplace<Position>(door, sf::Vector2i{ x,y }, this, *entity_level_map, door);
                }
                else if (structure.at({ x,y - 1 }).type == TILE_TYPE::WALL && structure.at({ x,y + 1 }).type == TILE_TYPE::WALL)
                {
                    auto door = createDoorSide(*registry);
                    registry->emplace<Position>(door, sf::Vector2i{ x,y }, this, *entity_level_map, door);
                }
            }
        }
    }
}

Level::Level(const Level_params& params, entt::registry& registry, std::unordered_map<const Level*, Quadtree<entt::entity>>& entity_level_map)
  : structure(createLevelStructure(params.structure_params)),
    reveal_mask(params.structure_params.level_size),
    entity_level_map(&entity_level_map),
    registry(&registry)
{
    entity_level_map[this] = Quadtree<entt::entity>({ {0,0}, {structure.getSize()} });
    placeDoors();
}

Level::~Level()
{
    auto& entities_qt = (*entity_level_map)[this];
    auto entities = entities_qt.find(entities_qt.getArea());
    for (auto entity : entities) registry->destroy(entity->second);

    entity_level_map->erase(entity_level_map->find(this));
}

void Level::updateVisibleTiles(const std::unordered_map<sf::Vector2i, Tile_visibility_info>& visible_tiles, const sf::RenderTarget& rt)
{
    const auto visible_tiles_vec = std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>{ visible_tiles.begin(), visible_tiles.end() };

    reveal_mask.reveal(visible_tiles_vec);
    if(view_range_overlay) view_range_overlay->update(*this, visible_tiles_vec, reveal_mask, rt);
}

bool Level::isPassable(const sf::Vector2i& position) const
{
    if (!structure.isPositionValid(position) || structure.at(position).type == TILE_TYPE::WALL) return false;

    auto tile_entities = (*entity_level_map)[this].find(position);
    for (auto entity : tile_entities)
    {
        if (registry->all_of<Nonpassable>(entity->second)) return false;
    }
    return true;
}

bool Level::isOpaque(const sf::Vector2i& position) const
{
    if (!structure.isPositionValid(position) || structure.at(position).type == TILE_TYPE::WALL) return true;

    auto tile_entities = (*entity_level_map)[this].find(position);
    for (auto entity : tile_entities)
    {
        if (registry->all_of<Opaque>(entity->second)) return true;
    }
    return false;
}

const Level_structure& Level::getStructure() const
{
    return structure;
}

void Level::update(entt::registry& registry, World& world, sf::RenderTarget& rt)
{
    auto& entities_qt = (*entity_level_map)[this];

    const auto [tl, br] = visibleAreaBoundsTiles(rt.getView());
    auto visible_entities = entities_qt.find({ tl, br });
    for (const auto entity : visible_entities)
    {
        auto rc = registry.try_get<Render_component>(entity->second);
        if (rc)
        {
            for (auto& [zlevel, animations] : rc->zlevel_animation_map)
            {
                for (auto& animation : animations)
                {
                    animation.updateFrameIdx();
                }
            }
        }
    }

    auto entities = entities_qt.find(entities_qt.getArea());
    for (auto entity : entities)
    {       
        auto player = registry.try_get<Player>(entity->second);
        if (player)
        {
            updatePlayer(registry, entities_qt, world, *player);
        }
    }
}

void Level::loadVisuals()
{
    if(!tile_map) tile_map = std::make_unique<Level_tile_map>(structure, sf::Vector2f{ Tile_sprite_storage::tile_size }, sf::Vector2i{ 30,30 });
    if(!view_range_overlay) view_range_overlay = std::make_unique<View_range_overlay>();
}

void Level::unloadVisuals()
{
    tile_map = nullptr;
    view_range_overlay = nullptr;
}
