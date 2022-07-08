#pragma once
#include "../gfx/animated_sprite/animated_sprite.h"
#include <vector>
#include <map>


struct Render_component
{
    std::map<int, std::vector<Animated_sprite>> zlevel_animation_map;
};