#include "chest.h"
#include "../components/chest.h"
#include "../components/inventory.h"
#include "../components/render_component.h"
#include "../asset_storage/texture_bank.h"
#include "../gfx/zlevels.h"


entt::entity createChest(entt::registry& registry, unsigned int chest_size)
{
    auto chest = registry.create();
    registry.emplace<Chest>(chest);
    registry.emplace<Inventory>(chest, chest_size);

    auto chest_frames = std::make_shared<Animated_sprite_frames>(Texture_bank::getTexture("chest.png"), std::vector<sf::IntRect>{ {0,0,64,64} });
    Animated_sprite chest_animation{ chest_frames, 1 };
    chest_animation.setPosition(sf::Vector2f{ 0, -48 });
    registry.emplace<Render_component>(chest, Render_component{ { {zlevel::chest, { chest_animation } } }, { {zlevel::chest, { chest_animation } } } });

    return chest;
}