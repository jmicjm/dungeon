#pragma once
#include "SFML/Graphics/View.hpp"

#include <functional>
#include <chrono>

class View_follower
{
    std::chrono::steady_clock::time_point last_time;

    sf::Vector2f centerDst();
    sf::Vector2f edgeDst();
    void followVec(const sf::Vector2f& vec);

public:
    sf::View* view = nullptr;
    std::function<sf::Vector2f()> target_position;
    float velocity = 0;//negative -> instant move
    float edge_dst = 0;

    View_follower();

    void followCenter();
    void follow();
    void resetTime();
};