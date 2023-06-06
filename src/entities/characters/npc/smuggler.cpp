#include "smuggler.h"
#include "../../../components/npc.h"
#include "../../../components/character.h"
#include "../../../components/nonpassable.h"
#include "../../../components/opaque.h"
#include "../../../components/render_component.h"
#include "../../../components/position.h"
#include "../../../components/description.h"
#include "../../../gfx/zlevels.h"
#include "../../../asset_storage/texture_bank.h"
#include "../../../gfx/animated_sprite/animated_sprite.h"
#include "../../../asset_storage/tile_sprite_storage.h"
#include "../body_templates.h"


entt::entity createSmuggler(entt::registry& registry)
{
    auto smuggler = registry.create();
    registry.emplace<Smuggler>(smuggler);
    registry.emplace<Npc>(smuggler);
    registry.emplace<Character>(smuggler, updateSmuggler);
    registry.emplace<Nonpassable>(smuggler);
    registry.emplace<Opaque>(smuggler);
    registry.emplace<Description>(smuggler, "smuggler");
    registry.emplace<Body>(smuggler, createHumanoidBody());

    static auto animation_frames = std::make_shared<Animated_sprite_frames>(Texture_bank::getTexture("characters/smuggler/smuggler.png"), std::vector<sf::IntRect>{ {0, 0, 64, 64} });
    Animated_sprite animation(animation_frames, 1);
    animation.setPosition(-sf::Vector2f(0, Tile_sprite_storage::tile_size.y / 2));
    registry.emplace<Render_component>(smuggler, Render_component{ { { zlevel::character, { animation } } } });

    return smuggler;
}

bool updateSmuggler(entt::registry& registry, World& world, const entt::entity entity)
{
    return true;
}
