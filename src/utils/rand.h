#pragma once
#include <random>
#include <type_traits>


template<typename T, typename Y>
std::common_type_t<T, Y> rand(T min, Y max)
{
    static std::mt19937 rng(std::random_device{}());

    using return_t = std::common_type_t<T, Y>;

    if constexpr (std::is_floating_point_v<return_t>)
    {
        return std::uniform_real_distribution<return_t>(min, max)(rng);
    }
    else
    {
        return std::uniform_int_distribution<return_t>(min, max)(rng);
    }
}