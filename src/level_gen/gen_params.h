#pragma once
#include "SFML/System/Vector2.hpp"

struct Gen_params
{
    sf::Vector2i level_size;
    sf::Vector2i max_room_size;
    sf::Vector2i min_room_size;
    unsigned int max_hallway_segment_length;
    unsigned int min_hallway_segment_length;
    unsigned int max_hallway_segment_count;
    unsigned int min_hallway_segment_count;
    sf::Vector2i max_empty_area_size;
};