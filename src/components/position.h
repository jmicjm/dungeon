#pragma once
#include "SFML/System/Vector2.hpp"
#include "../level/level.fwd.h"
#include "../utils/quadtree.h"

#include <entt/entt.hpp>

#include <unordered_map>


class Position
{
    sf::Vector2i coords = {0,0};
    Level* level = nullptr;
    const Quadtree<entt::entity>::value_type* qt_ptr = nullptr;
    std::unordered_map<const Level*, Quadtree<entt::entity>>* elvl_map = nullptr;

    Position(const Position& other) = delete;
    Position& operator=(const Position& other) = delete;

public:
    Position(const sf::Vector2i& coords, Level* level, std::unordered_map<const Level*, Quadtree<entt::entity>>& elvl_map, entt::entity entity);
    Position(Position&& other) noexcept;
    Position& operator=(Position&& other) noexcept;
    ~Position();

    void setPosition(const sf::Vector2i& coords, Level* level);
    void setCoords(const sf::Vector2i& coords);
    const sf::Vector2i& getCoords() const;
    Level* getLevel() const;

    Position replicate(entt::entity other) const;
};