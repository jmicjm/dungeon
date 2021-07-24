#pragma once
#include <random>
#include <type_traits>


template<typename T, typename Y>
typename std::common_type<T, Y>::type rand(T min, Y max)
{
    static std::random_device dev;
    static std::mt19937 rng(dev());

    if constexpr (std::is_floating_point<typename std::common_type<T, Y>::type>::value)
    {
        return std::uniform_real_distribution<>(min, max)(rng);
    }
    else
    {
        return std::uniform_int_distribution<>(min, max)(rng);
    }
}