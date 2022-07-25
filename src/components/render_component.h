#pragma once
#include "../gfx/animated_sprite/animated_sprite.h"
#include <vector>
#include <map>


struct Render_component
{
    using zlevel_map_type = std::map<int, std::vector<Animated_sprite>>;

    zlevel_map_type zlevel_animation_map;
    zlevel_map_type shadow_zlevel_animation_map;
};