#include "view_follower.h"

#include <algorithm>

void view_follower::followCenter(sf::View& view)
{
	if (target_position)
	{
		std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
		std::chrono::milliseconds t_diff = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_time);
		last_time = current_time;

		const sf::Vector2f vec = target_position() - view.getCenter();
		const float length = sqrt(vec.x * vec.x + vec.y * vec.y);
		if (length != 0)
		{
			const sf::Vector2f vec_norm = vec / length;

			float dst = velocity * t_diff.count() / 1000;

			sf::Vector2f offset =
			{
				std::clamp(vec_norm.x * dst,-abs(vec.x), abs(vec.x)),
				std::clamp(vec_norm.y * dst,-abs(vec.y), abs(vec.y))
			};

			view.move(offset);
		}
	}
}

void view_follower::resetTime()
{
	last_time = std::chrono::steady_clock::now();
}
