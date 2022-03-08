#pragma once
#include "SFML/System/Vector2.hpp"
#include <type_traits>
#include <stdexcept>


struct Level_structure_params
{
    template<typename T>
    class Minmax
    {
        T min, max;

    public:
        Minmax() : Minmax(T{}, T{}) {}
        Minmax(T min, T max) : min(validateMin(min, max)), max(max) {}

        const T& getMin() { return min; }
        const T& getMax() { return max; }

    private:
        T validateMin(const T& min, const T& max)
        {
            const bool valid = [&]
            {
                if constexpr (std::is_arithmetic_v<T>) return min <= max;
                else if constexpr (is_sf_vector2<T>::value) return min.x <= max.x && min.y <= max.y;
                else static_assert(!std::is_same_v<T, T>, "unsupported type");
            }();

            if (!valid) throw std::invalid_argument("max is less than min");
            return min;
        }

        template<typename Y>
        struct is_sf_vector2 : std::false_type {};
        template<typename Y>
        struct is_sf_vector2<sf::Vector2<Y>> : std::true_type {};
    };

    sf::Vector2i level_size;
    Minmax<sf::Vector2i> room_size;
    Minmax<unsigned int> hallway_segment_length;
    Minmax<unsigned int> hallway_segment_count;
    sf::Vector2i max_empty_area_size;
};