#include "view_follower.h"

#include <algorithm>
#include <cmath>

sf::Vector2f View_follower::centerDst()
{
	return (target_position && view) ? target_position() - view->getCenter() : sf::Vector2f{0,0};
}

sf::Vector2f View_follower::edgeDst()
{
	if (target_position && view)
	{
		const sf::Vector2f border = sf::Vector2f{ edge_dst, edge_dst };

		const sf::Vector2f tl = target_position() - view->getSize() / 2.f + border;
		const sf::Vector2f br = target_position() + view->getSize() / 2.f - border;

		const sf::Vector2f center = view->getCenter();

		const float x_dst = [&]()
		{
			if (2 * border.x >= view->getSize().x) return centerDst().x;
			if (center.x >= tl.x && center.x <= br.x) return 0.f;
			const float x_dst_top = tl.x - center.x;
			const float x_dst_bottom = br.x - center.x;
			return x_dst_top >= 0 ? std::min(x_dst_top, x_dst_bottom) : std::max(x_dst_top, x_dst_bottom);
		}();

		const float y_dst = [&]()
		{
			if (2 * border.y >= view->getSize().y) return centerDst().y;
			if (center.y >= tl.y && center.y <= br.y) return 0.f;
			const float y_dst_top = tl.y - center.y;
			const float y_dst_bottom = br.y - center.y;
			return y_dst_top >= 0 ? std::min(y_dst_top, y_dst_bottom) : std::max(y_dst_top, y_dst_bottom);
		}();

		return sf::Vector2f{ x_dst, y_dst };
	}
	return sf::Vector2f{ 0,0 };
}

void View_follower::followVec(const sf::Vector2f& vec)
{
	if (target_position && view)
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

				view->move(offset);
			}
			else
			{
				view->move(vec);
			}
		}
	}
}

View_follower::View_follower()
{
	resetTime();
}

void View_follower::followCenter()
{
	followVec(centerDst());
}

void View_follower::follow()
{
	followVec(edgeDst());
}

void View_follower::resetTime()
{
	last_time = std::chrono::steady_clock::now();
}
