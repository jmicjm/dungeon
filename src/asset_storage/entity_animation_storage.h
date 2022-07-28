#pragma once
#include "../utils/sf_vector2_hash.h"
#include "entity_animation_enum.h"
#include "../gfx/animated_sprite/animated_sprite.h"
#include "SFML/System/Vector2.hpp"
#include <unordered_map>


class Entity_animation_storage
{
    using move_animations_map_t = std::unordered_map<ENTITY_ANIMATION, std::unordered_map<sf::Vector2i, std::pair<int, Animated_sprite>>>;
    static move_animations_map_t move_animations;

    inline static std::pair<int, Animated_sprite> fallback_animation{ 0, { std::make_shared<Animated_sprite_frames>(), 1} };

public:
    static std::pair<int, Animated_sprite> getMoveAnimation(const ENTITY_ANIMATION animation_id, const sf::Vector2i& direction);
};