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
#include "../components/pending_animation.h"
#include "../gfx/zlevels.h"

#include <algorithm>


void Level::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    if (tile_map) rt.draw(*tile_map, st);

    std::map<int, std::vector<std::tuple<sf::Vector2i, Animated_sprite*, float>>> zlevel_map;

    const auto [tl, br] = visibleAreaBoundsTiles(rt.getView());
    getEntities().forEach({ tl, br }, [&](auto& entity) {
        const auto rc = registry.try_get<Render_component>(entity.second);
        const auto pos = registry.try_get<Position>(entity.second);
        const auto pa = registry.try_get<Pending_animation>(entity.second);
        if (rc && pos)
        {
            const auto visible = visible_tiles.find(pos->getCoords()) != visible_tiles.end();
            const auto src_visible = pa ? visible_tiles.find(pa->src) != visible_tiles.end() : visible;

            auto& map = visible ? rc->zlevel_animation_map : rc->shadow_zlevel_animation_map;

            const auto opacity_mul = [&]
            {
                if (visible == src_visible) return 1.f;

                if (auto it = map.find(pa->tracked_zlevel); it != map.end())
                {
                    auto max_perc = 0.f;
                    for (const auto& anim : it->second)
                    {
                        if (!anim.isRepeating() && anim.frameCount()) max_perc = std::max(max_perc, static_cast<float>(anim.getFrameIdx()) / anim.frameCount());
                    }
                    return src_visible ? 1 - max_perc : max_perc;
                }
                return 1.f;
            }();

            for (auto& [zlevel, animations] : map)
            {
                for (auto& animation : animations)
                {
                    zlevel_map[zlevel].push_back({ pos->getCoords(), &animation, opacity_mul });
                }
            }
        }
    });

    auto pixelPosition = [](const sf::Vector2i& tile_position)
    {
        return vecMul(sf::Vector2f{ tile_position }, Tile_sprite_storage::tile_size);
    };

    bool shadow_drew = false;
    for (auto& [zlevel, animations] : zlevel_map)
    {
        std::ranges::stable_sort(animations, [&](const auto& a, const auto& b) {
            return std::get<sf::Vector2i>(a).y * structure.getSize().x + std::get<sf::Vector2i>(a).x < std::get<sf::Vector2i>(b).y * structure.getSize().x + std::get<sf::Vector2i>(b).x; 
        });

        if (!shadow_drew && zlevel >= zlevel::character)
        {
            if (view_range_overlay) rt.draw(*view_range_overlay);
            shadow_drew = true;
        }
        for (const auto& [pos, animation, opacity_mul] : animations)
        {
            sf::RenderStates new_st = st;
            new_st.transform.translate(pixelPosition(pos));

            const auto old_color = animation->getColor();
            auto new_color = old_color;
            new_color.a *= opacity_mul;
            animation->setColor(new_color);

            rt.draw(*animation, new_st);

            animation->setColor(old_color);
        }
    }
    if (!shadow_drew && view_range_overlay) rt.draw(*view_range_overlay);
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
                    auto door = createDoorFront(registry);
                    registry.emplace<Position>(door, sf::Vector2i{ x,y }, this, entity_level_map, door);
                }
                else if (structure.at({ x,y - 1 }).type == TILE_TYPE::WALL && structure.at({ x,y + 1 }).type == TILE_TYPE::WALL)
                {
                    auto door = createDoorSide(registry);
                    registry.emplace<Position>(door, sf::Vector2i{ x,y }, this, entity_level_map, door);
                }
            }
        }
    }
}

Level::Level(const Level_params& params, entt::registry& registry, std::unordered_map<const Level*, Quadtree<entt::entity>>& entity_level_map)
  : structure(createLevelStructure(params.structure_params)),
    reveal_mask(params.structure_params.level_size),
    entity_level_map(entity_level_map),
    registry(registry)
{
    entity_level_map[this] = Quadtree<entt::entity>({ {0,0}, {structure.getSize()} });
    placeDoors();
}

Level::~Level()
{
    auto entities = getEntities().find(getEntities().getArea());
    for (auto entity : entities) registry.destroy(entity->second);

    entity_level_map.erase(entity_level_map.find(this));
}

void Level::updateVisibleTiles(const std::unordered_map<sf::Vector2i, Tile_visibility_info>& visible_tiles, const sf::RenderTarget& rt)
{
    this->visible_tiles = visible_tiles;
    const auto visible_tiles_vec = std::vector<std::pair<sf::Vector2i, Tile_visibility_info>>{ visible_tiles.begin(), visible_tiles.end() };

    reveal_mask.reveal(visible_tiles_vec);
    if(view_range_overlay) view_range_overlay->update(*this, visible_tiles_vec, reveal_mask, rt);
}

auto Level::getVisibleTiles() const -> const decltype(visible_tiles)&
{
    return visible_tiles;
}

bool Level::isPassable(const sf::Vector2i& position) const
{
    if (!structure.isPositionValid(position) || structure.at(position).type == TILE_TYPE::WALL) return false;

    return !getEntities().forEachUntil(position, [&](auto& entity) {
        return registry.all_of<Nonpassable>(entity.second);
    });
}

bool Level::isOpaque(const sf::Vector2i& position) const
{
    if (!structure.isPositionValid(position) || structure.at(position).type == TILE_TYPE::WALL) return true;

    return getEntities().forEachUntil(position, [&](auto& entity) {
        return registry.all_of<Opaque>(entity.second);
    });
}

const Level_structure& Level::getStructure() const
{
    return structure;
}

const Quadtree<entt::entity>& Level::getEntities() const
{
    return entity_level_map[this];
}

const entt::registry& Level::getRegistry() const
{
    return registry;
}

void Level::update(sf::RenderTarget& rt)
{
    const auto [tl, br] = visibleAreaBoundsTiles(rt.getView());
    getEntities().forEach({ tl, br }, [&](auto& entity) {
        if (auto rc = registry.try_get<Render_component>(entity.second))
        {
            for (auto& [zlevel, animations] : rc->zlevel_animation_map)
            {
                for (auto& animation : animations)
                {
                    animation.updateFrameIdx();
                }
            }
            for (auto& [zlevel, animations] : rc->shadow_zlevel_animation_map)
            {
                for (auto& animation : animations)
                {
                    animation.updateFrameIdx();
                }
            }
        }
    });
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
