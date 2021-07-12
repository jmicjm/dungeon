#pragma once
#include <type_traits>

template <typename T>
struct Vec2
{
    T x, y;
    Vec2() : Vec2(0, 0) {}
    Vec2(T X, T Y) : x(X), y(Y) {}

    template <typename Y>
    Vec2(const Vec2<Y>& r)
    {
        x = r.x;
        y = r.y;
    }

    template <typename Y>
    Vec2<T>& operator=(Vec2<Y> r)
    {
        x = r.x;
        y = r.y;
        return *this;
    }

    template <typename Y>
    Vec2<T>& operator+=(Vec2<Y> r)
    {
        x += r.x;
        y += r.y;
        return *this;
    }
    template <typename Y>
    Vec2<T>& operator-=(Vec2<Y> r)
    {
        x -= r.x;
        y -= r.y;
        return *this;
    }

    template<typename Y>
    Vec2<T>& operator*=(Y r)
    {
        x *= r;
        y *= r;
        return *this;
    }
    template<typename Y>
    Vec2<T>& operator/=(Y r)
    {
        x /= r;
        y /= r;
        return *this;
    }
};

template <typename T, typename Y>
bool operator==(Vec2<T> l, Vec2<Y> r)
{
    return l.x == r.x && l.y == r.y;
}
template <typename T, typename Y>
bool operator!=(Vec2<T> l, Vec2<Y> r)
{
    return l.x != r.x || l.y != r.y;
}

template <typename T, typename Y>
inline Vec2<typename std::common_type<T, Y>::type> operator+(Vec2<T> l, Vec2<Y> r)
{
    return Vec2<typename std::common_type<T, Y>::type>(l.x + r.x, l.y + r.y);
}
template <typename T, typename Y>
inline Vec2<typename std::common_type<T, Y>::type> operator-(Vec2<T> l, Vec2<Y> r)
{
    return Vec2<typename std::common_type<T, Y>::type>(l.x - r.x, l.y - r.y);
}

template<typename T, typename Y>
inline Vec2<typename std::common_type<T, Y>::type> operator*(Vec2<T> v, Y r)
{
    return Vec2<typename std::common_type<T, Y>::type>(v.x*r, v.y*r);
}
template<typename T, typename Y>
inline Vec2<typename std::common_type<T, Y>::type> operator/(Vec2<T> v, Y r)
{
    return Vec2<typename std::common_type<T, Y>::type>(v.x/r, v.y/r);
}

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
