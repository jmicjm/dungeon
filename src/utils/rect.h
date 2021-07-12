#pragma once
#include "vec2.h"

#include <type_traits>

template <typename T>
struct Rect
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
bool operator==(const Rect<T>& a, const Rect<T>& b)
{
	return a.tl == b.tl && a.br == b.br;
}
template<typename T>
bool operator!=(const Rect<T>& a, const Rect<T>& b)
{
	return !(a == b);
}

using Rect_i = Rect<Vec2i>;
using Rect_f = Rect<Vec2f>;