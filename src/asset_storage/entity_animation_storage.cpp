#include "entity_animation_storage.h"
#include "tile_sprite_storage.h"
#include "texture_bank.h"
#include "../gfx/zlevels.h"
#include <string>


Entity_animation_storage::move_animations_map_t Entity_animation_storage::move_animations = []
{
    using enum ENTITY_ANIMATION;
    using namespace zlevel;

    struct Info
    {
        ENTITY_ANIMATION entity_id;
        sf::Vector2i direction;
        std::string filename;
        int zlevel;
        sf::IntRect frame_size;
        unsigned int frame_count;
        unsigned int fps;
    };

    std::vector<Info> animations = 
    {
        #include "entity_move_animation_list.h"
    };

    move_animations_map_t map;

    for (auto& info : animations)
    {
        auto frames = [&]
        {
            const sf::Texture* tex = Texture_bank::getTexture(info.filename);
            std::vector<sf::IntRect> rects;
            auto rect = info.frame_size;
            for (int i = 0; i < info.frame_count; i++)
            {
                rects.push_back(rect);
                rect.left += rect.width;
            }
            return std::make_shared<Animated_sprite_frames>(tex, rects);
        }();

        auto animation = Animated_sprite{ frames, info.fps };
        sf::Vector2f offset = 
        {
            info.direction.x > 0 ? -info.direction.x * Tile_sprite_storage::tile_size.x : 0.f,
            info.direction.y > 0 ? -info.direction.y * Tile_sprite_storage::tile_size.y : 0.f,
        };

        animation.setPosition(offset - sf::Vector2f(0, Tile_sprite_storage::tile_size.y / 2));
        animation.setRepeat(false);
        map[info.entity_id].emplace(info.direction, std::pair{ info.zlevel, std::move(animation) });
    }

    return map;
}();

std::pair<int, Animated_sprite> Entity_animation_storage::getMoveAnimation(const ENTITY_ANIMATION animation_id, const sf::Vector2i& direction)
{
    if (auto it = move_animations.find(animation_id); it != move_animations.end())
    {
        const auto& direction_map = it->second;
        if (auto it = direction_map.find(direction); it != direction_map.end())
        {
            return it->second;
        }
    }
    return fallback_animation;
}
