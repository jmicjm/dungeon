#include "item_rc_storage.h"
#include "texture_bank.h"
#include "SFML/Graphics/Rect.hpp"
#include "../gfx/zlevels.h"
#include <vector>
#include <string>

using namespace items;

std::unordered_map<Item_id, Render_component> Item_rc_storage::rc_map = []
{
    decltype(rc_map) map;

    struct Info
    {
        Item_id id;
        std::string filename;
        sf::IntRect frame_area;
        unsigned int frame_count;
        unsigned int fps;
    };
    std::vector<Info> animations =
    {
        #include "item_rc_list.h"
    };

    for (auto& info : animations)
    {
        auto frames = [&]
        {
            const sf::Texture* tex = Texture_bank::getTexture(info.filename);
            std::vector<sf::IntRect> rects;
            auto rect = info.frame_area;
            for (int i = 0; i < info.frame_count; i++)
            {
                rects.push_back(rect);
                rect.left += rect.width;
            }
            return std::make_shared<Animated_sprite_frames>(tex, rects);
        }();

        map.insert({ info.id, Render_component{ { { zlevel::item, { Animated_sprite{ frames, info.fps } } } } } });
    }
    return map;
}();

Render_component Item_rc_storage::getRc(const Item_id id)
{
    if (auto it = rc_map.find(id); it != rc_map.end())
    {
        return it->second;
    }
    return Render_component{ { { zlevel::item, { Animated_sprite{ std::make_shared<Animated_sprite_frames>(), 1 } } } } };
}
