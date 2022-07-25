#include "infected_zombie.h"
#include "../../components/npc.h"
#include "../../components/character.h"
#include "../../components/nonpassable.h"
#include "../../components/opaque.h"
#include "../../components/render_component.h"
#include "../../components/position.h"
#include "../../gfx/zlevels.h"
#include "../../asset_storage/texture_bank.h"
#include "../../gfx/animated_sprite/animated_sprite.h"
#include "../../asset_storage/tile_sprite_storage.h"
#include "../../utils/rand.h"
#include "../../level/moveEntity.h"


entt::entity createInfectedZombie(entt::registry& registry)
{
    auto infected = registry.create();
    registry.emplace<Infected_zombie>(infected);
    registry.emplace<Npc>(infected);
    registry.emplace<Character>(infected, updateInfectedZombie);
    registry.emplace<Nonpassable>(infected);
    registry.emplace<Opaque>(infected);

    static auto animation_frames = std::make_shared<Animated_sprite_frames>(Texture_bank::getTexture("characters/infected.png"), std::vector<sf::IntRect>{ {0, 0, 64, 64} });
    Animated_sprite animation(animation_frames, 1);
    animation.setPosition(-sf::Vector2f(0, Tile_sprite_storage::tile_size.y / 2));
    registry.emplace<Render_component>(infected, Render_component{ { { zlevel::character, { animation } } } });

    return infected;
}

bool updateInfectedZombie(entt::registry& registry, World& world, const entt::entity entity)
{
    if (auto position = registry.try_get<Position>(entity))
    {
        moveEntity(registry, position->getLevel()->getEntities(), *position, sf::Vector2i{ rand(-1, 1), rand(-1, 1) });
    }
    return true;
}