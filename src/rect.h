#pragma once
#include "vec2.h"

template <typename T>
struct rect
{
	T tl, br;//top-left, bottom-right

	T size() const { return { br.x - tl.x + 1, br.y - tl.y + 1 }; }
	void move(const T offset)
	{
		tl += offset;
		br += offset;
	}
};

using rect_i = rect<vec2i>;
using rect_f = rect<vec2f>;