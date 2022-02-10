#pragma once
#include "SFML/System/Vector2.hpp"
#include <type_traits>
#include <cmath>

template<typename T, typename Y>
auto vecMul(const T& veca, const Y& vecb) -> sf::Vector2<typename std::common_type<decltype(veca.x), decltype(vecb.x)>::type>
{
    return { veca.x * vecb.x, veca.y * vecb.y };
};

template<typename T, typename Y>
auto vecDiv(const T& veca, const Y& vecb) -> sf::Vector2<typename std::common_type<decltype(veca.x), decltype(vecb.x)>::type>
{
    return { veca.x / vecb.x, veca.y / vecb.y };
};

template<typename T>
auto vecFloor(const sf::Vector2<T>& vec) -> sf::Vector2<T>
{
    return { std::floor(vec.x), std::floor(vec.y) };
}

template<typename T>
auto vecCeil(const sf::Vector2<T>& vec) -> sf::Vector2<T>
{
    return { std::ceil(vec.x), std::ceil(vec.y) };
}