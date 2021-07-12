#pragma once
#include "SFML/Graphics/View.hpp"

#include <functional>
#include <chrono>

class view_follower
{
	std::chrono::steady_clock::time_point last_time;
public:
	std::function<sf::Vector2f()> target_position;
	float velocity = 0;

	void followCenter(sf::View& view);
	void resetTime();
};