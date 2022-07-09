#include "world.h"
#include "../level/level.h"
#include "../level/level_params.h"
#include "../asset_storage/texture_bank.h"
#include "../components/position.h"
#include "../components/portal.h"
#include "../components/render_component.h"
#include "../gfx/zlevels.h"
#include "../level/visibleTiles.h"
#include "../global/window.h"

#include <vector>


void World::createPlayer()
{
    player = ::createPlayer(registry);
    registry.emplace<Position>(player, current_level->getStructure().getRoomRect(0).tl, current_level.get(), entity_level_map, player);
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
    rt.draw(*current_level, st);
}

World::World(const World_params& params)
{
    for (const auto& lp : params.level_params)
    {
        levels.push_back(std::make_shared<Level>(lp, registry, entity_level_map));
    }

    auto createPortal = [&](const sf::Texture* tex, const sf::IntRect& tex_rect, Portal portal, Level* dst_level, const sf::Vector2i& dst_coords)
    {
        auto entity = registry.create();
        registry.emplace<Portal>(entity, portal);
        registry.emplace<Position>(entity, dst_coords, dst_level, entity_level_map, entity);

        auto frames = std::make_shared<Animated_sprite_frames>(tex, std::vector<sf::IntRect>{ tex_rect });
        registry.emplace<Render_component>(entity, Render_component{ { { zlevel::portal, { Animated_sprite{frames, 1} } } } });
    };

    for (int i = 0; i < levels.size()-1; i++)
    {
        createPortal(Texture_bank::getTexture("assets/tiles/entrance_down.png"), { 0,0,64,64 }, Portal{ levels[i + 1], levels[i + 1]->getStructure().getRoomRect(0).tl }, levels[i].get(), levels[i]->getStructure().getRoomRect(0).tl);
    }
    for (int i = 1; i < levels.size(); i++)
    {
        createPortal(Texture_bank::getTexture("assets/tiles/entrance_up.png"), { 0,0,64,64 }, Portal{ levels[i - 1], levels[i - 1]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 } }, levels[i].get(), levels[i]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 });
    }
    for (auto l : levels)
    {
        createPortal(Texture_bank::getTexture("assets/tiles/entrance_portal.png"), { 0,0,64,64 }, Portal{ l, l->getStructure().getRoomRect(1).tl + sf::Vector2i{ 0,1 } }, l.get(), l->getStructure().getRoomRect(0).tl + sf::Vector2i{ 0,1 });
        createPortal(Texture_bank::getTexture("assets/tiles/entrance_portal.png"), { 0,0,64,64 }, Portal{ l, l->getStructure().getRoomRect(0).tl + sf::Vector2i{ 0,1 } }, l.get(), l->getStructure().getRoomRect(1).tl + sf::Vector2i{ 0,1 });
    }

    current_level = *levels.begin();
    current_level->loadVisuals();
    createPlayer();
    initViewFollowers();
}

void World::update(sf::RenderTarget& rt)
{
    current_level->update(registry, *this, rt);

    auto view = rt.getView();
    view = vf.follow(view);
    view = vf_instant.follow(view);
    sf::View rounded_view = view;
    sf::Vector2f tl{ sf::Vector2i{ view.getCenter() - view.getSize() / 2.f } };
    rounded_view.setCenter(tl + view.getSize() / 2.f);
    rt.setView(rounded_view);//use rounded view to avoid rendering at non integer positions

    auto& pos = registry.get<Position>(player);
    auto vis_tiles = visibleTiles(pos.getCoords(), *current_level);
    current_level->updateVisibleTiles(std::move(vis_tiles), rt);
}

std::shared_ptr<Level> World::changeLevel(std::shared_ptr<Level> new_level)
{
    if (current_level) current_level->unloadVisuals();
    if (new_level) new_level->loadVisuals();
    window.setView(vf_instant.followCenter(window.getView()));

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
