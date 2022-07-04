#include "world.h"
#include "../level/level.h"
#include "../level/level_params.h"
#include "../asset_storage/texture_bank.h"
#include "../components/position.h"
#include "../level/visibleTiles.h"
#include "../global/window.h"

#include <vector>


void World::createPlayer()
{
    player = ::createPlayer(registry);
    registry.emplace<Position>(player, current_level->getStructure().getRoomRect(0).tl, current_level.get(), entity_level_map, player);
}

void World::addLevel(std::shared_ptr<Level> level)
{
    entity_level_map.insert({ level.get(), Quadtree<entt::entity>{ { { 0,0 }, level->getStructure().getSize()} } });
    levels.insert(std::move(level));
}

void World::removeLevel(const std::shared_ptr<Level>& level)
{
    if (auto it = levels.find(level); it != levels.end())
    {
        levels.erase(it);
    }
    if (auto it = entity_level_map.find(level.get()); it != entity_level_map.end())
    {
        entity_level_map.erase(it);
    }
    //todo: do something with entities associated with level
}

void World::initViewFollowers()
{
    vf.target_position = [&]() { return sf::Vector2f(registry.get<Position>(player).getCoords()) * 64.f + sf::Vector2f(32, 0); };
    vf.velocity = 300;
    vf.edge_dst = 64 * 3 + 32;

    vf_instant = vf;
    vf_instant.velocity = -1;
    vf_instant.edge_dst = 32;
    window.setView(vf_instant.followCenter(window.getView()));
}

void World::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    current_level->draw(registry, entity_level_map.find(current_level.get())->second, rt, st);
}

World::World(const World_params& params)
{
    std::vector<std::shared_ptr<Level>> levels;

    for (const auto& lp : params.level_params)
    {
        auto level = std::make_shared<Level>(lp);
        levels.push_back(level);
        addLevel(std::move(level));
    }

    for (int i = 0; i < levels.size()-1; i++)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_down.png"), { 0,0,64,64 });
        levels[i]->entrances.insert({ levels[i]->getStructure().getRoomRect(0).tl, Entrance{ { levels[i + 1], levels[i + 1]->getStructure().getRoomRect(0).tl }, spr } } );
    }
    for (int i = 1; i < levels.size(); i++)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_up.png"), { 0,0,64,64 });
        levels[i]->entrances.insert({ levels[i]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 }, Entrance{ { levels[i - 1], levels[i - 1]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 } }, spr } });
    }
    for (auto l : levels)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_portal.png"), { 0,0,64,64 });
        l->entrances.insert({ l->getStructure().getRoomRect(0).tl + sf::Vector2i{ 0,1 }, Entrance{ { l, l->getStructure().getRoomRect(1).tl }, spr } });
        l->entrances.insert({ l->getStructure().getRoomRect(1).tl + sf::Vector2i{ 0,1 }, Entrance{ { l, l->getStructure().getRoomRect(0).tl }, spr } });
    }

    current_level = *levels.begin();
    current_level->loadVisuals();
    createPlayer();
    initViewFollowers();
}

void World::update(sf::RenderTarget& rt)
{
    current_level->update(registry, entity_level_map[current_level.get()], *this, rt);

    auto view = rt.getView();
    view = vf.follow(view);
    view = vf_instant.follow(view);
    sf::View rounded_view = view;
    sf::Vector2f tl{ sf::Vector2i{ view.getCenter() - view.getSize() / 2.f } };
    rounded_view.setCenter(tl + view.getSize() / 2.f);
    rt.setView(rounded_view);//use rounded view to avoid rendering at non integer positions

    auto& pos = registry.get<Position>(player);
    auto vis_tiles = visibleTiles(pos.getCoords(), *current_level.get(), entity_level_map[current_level.get()]);
    current_level->updateVisibleTiles(std::move(vis_tiles), rt);
}

std::shared_ptr<Level> World::changeLevel(std::shared_ptr<Level> new_level)
{
    if (current_level) current_level->unloadVisuals();
    if (new_level) new_level->loadVisuals();
    window.setView(vf_instant.followCenter(window.getView()));

    if (!levels.contains(new_level)) addLevel(new_level);

    return std::exchange(current_level, new_level);
}

entt::entity World::getPlayer() const
{
    return player;
}

const entt::registry& World::getRegistry() const
{
    return registry;
}
