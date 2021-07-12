#pragma once
#include "../utils/vec2.h"

struct Gen_params
{
	Vec2i level_size;
	Vec2i max_room_size;
	Vec2i min_room_size;
	unsigned int max_hallway_segment_length;
	unsigned int min_hallway_segment_length;
	unsigned int max_hallway_segment_count;
	unsigned int min_hallway_segment_count;
	Vec2i max_empty_area_size;
};