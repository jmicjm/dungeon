#pragma once
#include "SFML/System/Vector2.hpp"
#include <functional>

namespace std
{
    template<typename T>
    struct hash<sf::Vector2<T>>
    {
        size_t operator()(const sf::Vector2<T>& vec) const
        {
            return std::hash<T>{}(vec.x) ^ (std::hash<T>{}(vec.y) << 1);
        }
    };
}