#include "world.h"
#include "../level/level.h"
#include "../level/level_params.h"
#include "../asset_storage/texture_bank.h"
#include "../components/position.h"
#include "../components/portal.h"
#include "../components/character.h"
#include "../components/character_update_tags.h"
#include "../components/render_component.h"
#include "../components/pending_animation.h"
#include "../gfx/zlevels.h"
#include "../gfx/utils/visibleAreaBounds.h"
#include "../level/visibleTiles.h"
#include "../global/window.h"

#include <vector>

#include "../entities/npc/infected_zombie.h"
#include "../entities/npc/smuggler.h"
#include "../entities/gate.h"


void World::createPlayer()
{
    player = ::createPlayer(registry);
    registry.emplace<Position>(player, current_level->getStructure().getRoomRect(0).tl, current_level.get(), entity_level_map, player);
    registry.emplace<Active_character>(player);
}

void World::progressTurn(const sf::RenderTarget& rt)
{
    updatePendingAnimations(rt);
    if (registry.view<Pending_animation>().size()) return;

    auto updateCharacter = [&] (const auto entity)
    {
        const auto update_f = registry.get<Character>(entity).update;
        if (!update_f) return true;
        return update_f(registry, *this, entity);
    };

    for (const auto entity : registry.view<Character_updating>())
    {
        if (!updateCharacter(entity)) return;
        else registry.erase<Character_updating>(entity);
        updatePendingAnimations(rt);
        if (registry.view<Pending_animation>().size()) return;
    }

    for (const auto entity : registry.view<Character_awaiting_update>())
    {
        registry.erase<Character_awaiting_update>(entity);
        if (!updateCharacter(entity))
        {
            registry.emplace<Character_updating>(entity);           
            return;
        }
        updatePendingAnimations(rt);
        if (registry.view<Pending_animation>().size()) return;
    }

    for (const auto entity : registry.view<Active_character>())
    {
        registry.emplace<Character_awaiting_update>(entity);
    }
}

void World::updatePendingAnimations(const sf::RenderTarget& rt)
{
    for (const auto& [entity, rc, pa] : registry.view<Render_component, Pending_animation>().each())
    {
        const auto isInvisible = [&]
        {
            const auto outOfScreen = [&] 
            {
                auto [tl, br] = visibleAreaBoundsTiles(rt.getView());
                tl -= {1, 1};
                br += {1, 1};
                auto contains = [&](const sf::Vector2i& point)
                {
                    return point.x >= tl.x && point.x < br.x
                        && point.y >= tl.y && point.y < br.y;
                };
                return !contains(pa.dst);
            };
            const auto validLevel = [&, pos = registry.try_get<Position>(entity)]{ return pos && pos->getLevel() == current_level.get(); };
            const auto& vtiles = current_level->getVisibleTiles();
            return outOfScreen() || !validLevel() || vtiles.find(pa.src) == vtiles.end() && vtiles.find(pa.dst) == vtiles.end();
        };
        const auto isRepeating = [&]
        {
            for (const auto& anim : rc.zlevel_animation_map[pa.tracked_zlevel])
            {
                if (!anim.isRepeating()) return false;
            }
            return true;
        };
        const auto isFinished = [&]
        {
            for (const auto& anim : rc.zlevel_animation_map[pa.tracked_zlevel])
            {
                if (!anim.isRepeating() && anim.getFrameIdx() == anim.frameCount() - 1) return true;
            }
            return false;
        };

        if (isInvisible() || isRepeating() || isFinished())
        {
            pa.restore(rc);
            registry.erase<Pending_animation>(entity);
        }
    }
    updateMovingFromGates(registry, current_level->getEntities());
}

void World::initViewFollowers()
{
    vf.target_position = [&]() { return sf::Vector2f(registry.get<Position>(player).getCoords()) * 64.f + sf::Vector2f(32, 0); };
    vf.velocity = 300;
    vf.screen_border = 192;
    vf.target_border = 32;

    vf_instant = vf;
    vf_instant.velocity = -1;
    vf_instant.screen_border = 0;
    window.setView(vf_instant.followCenter(window.getView()));
}

void World::spawnNpcs()
{
    auto infected = createInfectedZombie(registry);
    registry.emplace<Position>(infected, current_level->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 }, current_level.get(), entity_level_map, infected);
    registry.emplace<Active_character>(infected);

    auto smuggler = createSmuggler(registry);
    registry.emplace<Position>(smuggler, current_level->getStructure().getRoomRect(0).br, current_level.get(), entity_level_map, smuggler);
    registry.emplace<Active_character>(smuggler);
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
        createPortal(Texture_bank::getTexture("tiles/entrance_down.png"), { 0,0,64,64 }, Portal{ levels[i + 1], levels[i + 1]->getStructure().getRoomRect(0).tl }, levels[i].get(), levels[i]->getStructure().getRoomRect(0).tl);
    }
    for (int i = 1; i < levels.size(); i++)
    {
        createPortal(Texture_bank::getTexture("tiles/entrance_up.png"), { 0,0,64,64 }, Portal{ levels[i - 1], levels[i - 1]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 } }, levels[i].get(), levels[i]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 });
    }
    for (auto l : levels)
    {
        createPortal(Texture_bank::getTexture("tiles/entrance_portal.png"), { 0,0,64,64 }, Portal{ l, l->getStructure().getRoomRect(1).tl + sf::Vector2i{ 0,1 } }, l.get(), l->getStructure().getRoomRect(0).tl + sf::Vector2i{ 0,1 });
        createPortal(Texture_bank::getTexture("tiles/entrance_portal.png"), { 0,0,64,64 }, Portal{ l, l->getStructure().getRoomRect(0).tl + sf::Vector2i{ 0,1 } }, l.get(), l->getStructure().getRoomRect(1).tl + sf::Vector2i{ 0,1 });
    }

    changeLevel(*levels.begin());
    createPlayer();
    initViewFollowers();
    spawnNpcs();
}

void World::update(sf::RenderTarget& rt)
{
    current_level->update(rt);

    progressTurn(rt);

    auto view = rt.getView();
    view = vf.follow(view, rt);
    view = vf_instant.follow(view, rt);
    sf::View rounded_view = view;
    sf::Vector2f tl{ sf::Vector2i{ view.getCenter() - view.getSize() / 2.f } };
    rounded_view.setCenter(tl + view.getSize() / 2.f);
    rt.setView(rounded_view);//use rounded view to avoid rendering at non integer positions

    auto& pos = registry.get<Position>(player);
    auto vis_tiles = visibleTiles(pos.getCoords(), *current_level);
    current_level->updateVisibleTiles(std::move(vis_tiles), rt);
    updatePendingAnimations(rt);//drop animations that are invisible now
}

std::shared_ptr<Level> World::changeLevel(std::shared_ptr<Level> new_level)
{
    if (!new_level) return nullptr;;
    if (new_level != current_level)
    {
        auto activec_view = registry.view<Active_character>();
        registry.erase<Active_character>(activec_view.begin(), activec_view.end());

        for (const auto entity : new_level->getEntities().find(new_level->getEntities().getArea()))
        {
            if(registry.all_of<Character>(entity->second)) registry.emplace<Active_character>(entity->second);
        }
    }

    if (current_level) current_level->unloadVisuals();
    new_level->loadVisuals();
    window.setView(vf_instant.followCenter(window.getView()));

    return std::exchange(current_level, new_level);
}

entt::entity World::getPlayer() const
{
    return player;
}

auto World::getEntities() const -> const decltype(entity_level_map)&
{
    return entity_level_map;
}

entt::registry& World::getRegistry()
{
    return registry;
}
