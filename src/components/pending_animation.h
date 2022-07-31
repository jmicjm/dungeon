#pragma once
#include "render_component.h"


struct Pending_animation
{
    int tracked_zlevel;
    Render_component new_zlevels;
    sf::Vector2i src;
    sf::Vector2i dst;

    void restore(Render_component& rc)
    {
        static const auto mergeMaps = [](auto& src_map, auto& dst_map)
        {
            for (const auto& [zlevel, animations] : src_map)
            {
                if (animations.size()) dst_map[zlevel] = animations;
                else if (auto it = dst_map.find(zlevel); it != dst_map.end()) dst_map.erase(it);
            }
        };

        mergeMaps(new_zlevels.zlevel_animation_map, rc.zlevel_animation_map);
        mergeMaps(new_zlevels.shadow_zlevel_animation_map, rc.shadow_zlevel_animation_map);
    }
};
