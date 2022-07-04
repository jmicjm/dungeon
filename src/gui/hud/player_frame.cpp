#include "player_frame.h"
#include "../../utils/sf_vector2_utils.h"
#include "../../gfx/animated_sprite/animated_sprite.h"

#include <algorithm>


void gui::Player_frame::redraw()
{
    frame.draw();

    auto animation = registry->try_get<Animated_sprite>(player);
    if (animation)
    {
        sf::RenderStates st = [&]
        {
            sf::RenderStates st;

            float asize = std::min(size().x, size().y) * 0.91;
            auto scale = vecDiv(sf::Vector2f{asize,asize}, sf::Vector2f{ 64,64 });

            auto player_size = vecMul(sf::Vector2f{ 64,64 }, scale);
            sf::Vector2f offset = { size() / 2.f - player_size / 2.f };

            st.transform.translate(offset).scale(scale);
            return st;
        }();
        
        draw(*animation, st);
    }
}

gui::Player_frame::Player_frame()
{
    frame.setSizeInfo({ {0,0}, {1,1} });
    frame.setParent(this);
}

void gui::Player_frame::setPlayer(const entt::registry& registry, entt::entity player)
{
    this->registry = &registry;
    this->player = player;
}

void gui::Player_frame::setAppearance(const Frame_appearance& appearance)
{
    frame.setAppearance(appearance);
}
