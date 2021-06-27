#pragma once
#include "vec2.h"

#include <type_traits>

template <typename T>
struct rect
{
	T tl, br;//top-left, bottom-right

	T size() const 
	{
		if constexpr (std::is_floating_point<T>::value)
		{
			return { br.x - tl.x, br.y - tl.y };
		}
		else
		{
			return { br.x - tl.x + 1, br.y - tl.y + 1 };
		}
	}
	void move(const T offset)
	{
		tl += offset;
		br += offset;
	}
};

template<typename T>
bool operator==(const rect<T>& a, const rect<T>& b)
{
	return a.tl == b.tl && a.br == b.br;
}
template<typename T>
bool operator!=(const rect<T>& a, const rect<T>& b)
{
	return !(a == b);
}

using rect_i = rect<vec2i>;
using rect_f = rect<vec2f>;