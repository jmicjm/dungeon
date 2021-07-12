#pragma once
#include "SFML/Graphics/View.hpp"

#include <functional>
#include <chrono>

class view_follower
{
	std::chrono::steady_clock::time_point last_time;

	sf::Vector2f centerDst();
	sf::Vector2f edgeDst();
	void followVec(const sf::Vector2f& vec);

public:
	sf::View* view = nullptr;
	std::function<sf::Vector2f()> target_position;
	float velocity = 0;
	float edge_dst = 0;

	void followCenter();
	void follow();
	void resetTime();
};