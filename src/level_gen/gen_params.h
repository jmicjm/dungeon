#pragma once
#include "../utils/vec2.h"

struct gen_params
{
	vec2i max_room_size;
	vec2i min_room_size;
	unsigned int max_hallway_segment_length;
	unsigned int min_hallway_segment_length;
	unsigned int max_hallway_segment_count;
	unsigned int min_hallway_segment_count;
	unsigned int max_door_count;
	unsigned int min_door_count;
	vec2i max_empty_area;
};