#pragma once
#include "SFML/Graphics/View.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <functional>
#include <chrono>

class View_follower
{
    std::chrono::steady_clock::time_point last_time;

    sf::Vector2f centerDst(const sf::View& view);
    sf::Vector2f edgeDst(const sf::View& view, const sf::RenderTarget& rt);
    sf::View followVec(const sf::Vector2f& vec, sf::View view);

public:
    std::function<sf::Vector2f()> target_position;
    float velocity = 0;//negative -> instant move
    float screen_border = 0;
    float target_border = 0;

    View_follower();

    sf::View followCenter(sf::View view);
    sf::View follow(sf::View view, const sf::RenderTarget& rt);
    void resetTime();
};