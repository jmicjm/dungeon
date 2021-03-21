#pragma once
#include "vec2.h"

struct gen_params
{
	vec2i max_room_size;
	vec2i min_room_size;
	unsigned int max_hallway_length;
	unsigned int min_hallway_length;
};