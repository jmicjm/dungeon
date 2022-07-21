#include "../components/player.h"
#include "../components/character.h"
#include "../components/nonpassable.h"
#include "../components/render_component.h"
#include "../components/portal.h"
#include "portal.h"
#include "../gfx/zlevels.h"
#include "../asset_storage/texture_bank.h"
#include "../gfx/animated_sprite/animated_sprite.h"
#include "../components/position.h"
#include "../level/moveEntity.h"
#include "../world/world.h"
#include "../asset_storage/tile_sprite_storage.h"

#include "SFML/Window/Keyboard.hpp"

#include <memory>


entt::entity createPlayer(entt::registry& registry)
{
    auto player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Character>(player, updatePlayer);
    registry.emplace<Nonpassable>(player);

    std::shared_ptr<Animated_sprite_frames> player_frames = []()
    {
        const sf::Texture* tex = Texture_bank::getTexture("characters/wild_mage_frames.png");
        std::vector<sf::IntRect> rects;
        for (int i = 0; i < 16; i++)
        {
            rects.push_back(sf::IntRect(i * 64, 0, 64, 64));
        }
        return std::make_shared<Animated_sprite_frames>(tex, rects);
    }();
    Animated_sprite player_animation(player_frames, 16);
    player_animation.setPosition(-sf::Vector2f(0, Tile_sprite_storage::tile_size.y / 2));
    registry.emplace<Render_component>(player, Render_component{ { {zlevel::character, { player_animation } } } });

    return player;
}

bool updatePlayer(entt::registry& registry, World& world, const entt::entity entity)
{
    if (auto player = registry.try_get<Player>(entity))
    {
        std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
        if ((t - player->last_action_t) < std::chrono::milliseconds(200)) return false;
        player->last_action_t = t;
    }
    
    if (auto position = registry.try_get<Position>(entity))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            auto tile_entities = position->getLevel()->getEntities().find(position->getCoords());
            for (auto entity : tile_entities)
            {
                if (auto portal = registry.try_get<Portal>(entity->second))
                {
                    if (auto destination = portal->destination_level.lock())
                    {
                        usePortal(*portal, *position);
                        world.changeLevel(destination);
                        return true;
                    }
                }
            }
        }

        sf::Vector2i offset = { 0,0 };

        offset.x += sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        offset.x -= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        offset.y += sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        offset.y -= sf::Keyboard::isKeyPressed(sf::Keyboard::W);

        const sf::Vector2i old_pos = position->getCoords();
        moveEntity(registry, position->getLevel()->getEntities(), *position, offset);

        return old_pos != position->getCoords();
    }
    return true;
}
