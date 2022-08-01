#include "view_follower.h"
#include "../utils/sf_vector2_utils.h"

#include <algorithm>
#include <cmath>


sf::Vector2f View_follower::centerDst(const sf::View& view)
{
    return (target_position) ? target_position() - view.getCenter() : sf::Vector2f{0,0};
}

sf::Vector2f View_follower::edgeDst(const sf::View& view, const sf::RenderTarget& rt)
{
    if (target_position)
    {
        const sf::Vector2f border = vecDiv(view.getSize(), sf::Vector2f{ rt.getSize() }) * screen_border + sf::Vector2f{ target_border, target_border };

        const sf::Vector2f tl = target_position() - view.getSize() / 2.f + border;
        const sf::Vector2f br = target_position() + view.getSize() / 2.f - border;

        const sf::Vector2f center = view.getCenter();

        const float x_dst = [&]()
        {
            if (2 * border.x >= view.getSize().x) return centerDst(view).x;
            if (center.x >= tl.x && center.x <= br.x) return 0.f;
            const float x_dst_top = tl.x - center.x;
            const float x_dst_bottom = br.x - center.x;
            return x_dst_top >= 0 ? std::min(x_dst_top, x_dst_bottom) : std::max(x_dst_top, x_dst_bottom);
        }();

        const float y_dst = [&]()
        {
            if (2 * border.y >= view.getSize().y) return centerDst(view).y;
            if (center.y >= tl.y && center.y <= br.y) return 0.f;
            const float y_dst_top = tl.y - center.y;
            const float y_dst_bottom = br.y - center.y;
            return y_dst_top >= 0 ? std::min(y_dst_top, y_dst_bottom) : std::max(y_dst_top, y_dst_bottom);
        }();

        return sf::Vector2f{ x_dst, y_dst };
    }
    return sf::Vector2f{ 0,0 };
}

sf::View View_follower::followVec(const sf::Vector2f& vec, sf::View view)
{
    if (target_position)
    {
        std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
        std::chrono::milliseconds t_diff = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time);
        last_time = current_time;

        const float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        if (length != 0)
        {
            if (velocity >= 0)
            {
                const sf::Vector2f vec_norm = vec / length;
                const float dst = velocity * t_diff.count() / 1000;

                const sf::Vector2f offset =
                {
                    std::clamp(vec_norm.x * dst,-std::abs(vec.x), std::abs(vec.x)),
                    std::clamp(vec_norm.y * dst,-std::abs(vec.y), std::abs(vec.y))
                };

                view.move(offset);
            }
            else
            {
                view.move(vec);
            }
        }
    }
    return view;
}

View_follower::View_follower()
{
    resetTime();
}

sf::View View_follower::followCenter(sf::View view)
{
    return followVec(centerDst(view), view);
}

sf::View View_follower::follow(sf::View view, const sf::RenderTarget& rt)
{
   return followVec(edgeDst(view, rt), view);
}

void View_follower::resetTime()
{
    last_time = std::chrono::steady_clock::now();
}
