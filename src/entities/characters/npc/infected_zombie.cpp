#include "infected_zombie.h"
#include "../../../components/npc.h"
#include "../../../components/character.h"
#include "../../../components/nonpassable.h"
#include "../../../components/opaque.h"
#include "../../../components/render_component.h"
#include "../../../components/position.h"
#include "../../../components/move_animation_id.h"
#include "../../../components/description.h"
#include "../../../gfx/zlevels.h"
#include "../../../asset_storage/texture_bank.h"
#include "../../../gfx/animated_sprite/animated_sprite.h"
#include "../../../asset_storage/tile_sprite_storage.h"
#include "../../../utils/rand.h"
#include "../../../level/moveEntity.h"
#include "../body_templates.h"


Body createInfectedZombieBody(entt::registry& registry)
{
    using enum Body_part_type;

    auto mutilated_left_arm = Body_part{ JOINT, "left shoulder", true }.addChild(Body_part{ FOREARM, "left" });

    Body body;
    body.graph.root = std::make_unique<Body_part>(Body_part{ HEAD, registry, createHeadInventory() }
        .addChild(Body_part{ NECK }
            .addChild(Body_part{ UPPER_TORSO, registry, createUpperTorsoInventory() }
                .addChild(mutilated_left_arm)
                .addChild(createArm(registry, "right"))
                .addChild(Body_part{ LOWER_TORSO, registry, createLowerTorsoInventory() }
                    .addChild(createLeg(registry, "left"))
                    .addChild(createLeg(registry, "right"))
                )
            )
        )
    );

    return body;
}

entt::entity createInfectedZombie(entt::registry& registry)
{
    auto infected = registry.create();
    registry.emplace<Infected_zombie>(infected);
    registry.emplace<Npc>(infected);
    registry.emplace<Character>(infected, updateInfectedZombie);
    registry.emplace<Move_animation_id>(infected, ENTITY_ANIMATION::INFECTED_ZOMBIE);
    registry.emplace<Nonpassable>(infected);
    registry.emplace<Opaque>(infected);
    registry.emplace<Description>(infected, "infected zombie");
    registry.emplace<Body>(infected, createInfectedZombieBody(registry));

    static auto animation_frames = std::make_shared<Animated_sprite_frames>(Texture_bank::getTexture("characters/infected/infected.png"), std::vector<sf::IntRect>{ {0, 0, 64, 64} });
    Animated_sprite animation(animation_frames, 1);
    animation.setPosition(-sf::Vector2f(0, Tile_sprite_storage::tile_size.y / 2));
    registry.emplace<Render_component>(infected, Render_component{ { { zlevel::character, { animation } } } });

    return infected;
}

bool updateInfectedZombie(entt::registry& registry, World& world, const entt::entity entity)
{
    if (auto position = registry.try_get<Position>(entity))
    {
        moveEntity(registry, entity, sf::Vector2i{ rand(-1, 1), rand(-1, 1) });
    }
    return true;
}