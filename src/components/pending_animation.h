#pragma once
#include "render_component.h"


struct Pending_animation
{
    int tracked_zlevel;
    std::vector<Animated_sprite> new_animations;
    std::vector<Animated_sprite> new_shadow_animations;
    sf::Vector2i src;
    sf::Vector2i dst;
};