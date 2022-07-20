#pragma once
#include <vector>
#include <list>
#include <memory>
#include <utility>

#include "SFML/System/Vector2.hpp"


template<typename T>
class Quadtree
{
    using Vec2 = sf::Vector2i;

    struct Area
    {
        Vec2 tl, br;
        Vec2 size() const
        {
            return { br.x - tl.x, br.y - tl.y };
        }
        bool contains(const Vec2& point) const
        {
            return point.x >= tl.x && point.x < br.x&& point.y >= tl.y && point.y < br.y;
        }
        bool intersects(const Area& other) const
        {
            return tl.x <= other.br.x && br.x > other.tl.x
                && tl.y <= other.br.y && br.y > other.tl.y;
        };
    } area;

    constexpr static size_t max_size = 16;
public:
    using value_type = std::pair<const Vec2, T>;
private:
    std::list<value_type> elements;

    std::unique_ptr<Quadtree<T>> tl;
    std::unique_ptr<Quadtree<T>> tr;
    std::unique_ptr<Quadtree<T>> bl;
    std::unique_ptr<Quadtree<T>> br;

    void split();
    bool isLeaf() const;
    std::list<value_type> get();

public:
    Quadtree(const Area& area = { {0,0}, {0,0} })
        : area(area) {}
    Quadtree(const Quadtree<T>& other)
        : area(other.area),
        elements(other.elements),
        tl(other.tl ? std::make_unique<Quadtree<T>>(*other.tl) : nullptr),
        tr(other.tr ? std::make_unique<Quadtree<T>>(*other.tr) : nullptr),
        bl(other.bl ? std::make_unique<Quadtree<T>>(*other.bl) : nullptr),
        br(other.br ? std::make_unique<Quadtree<T>>(*other.br) : nullptr) {}
    Quadtree(Quadtree<T>&& other) = default;

    Quadtree<T>& operator=(const Quadtree<T>& other)
    {
        area = other.area;
        elements = other.elements;
        tl = other.tl ? std::make_unique<Quadtree<T>>(*other.tl) : nullptr;
        tr = other.tr ? std::make_unique<Quadtree<T>>(*other.tr) : nullptr;
        bl = other.bl ? std::make_unique<Quadtree<T>>(*other.bl) : nullptr;
        br = other.br ? std::make_unique<Quadtree<T>>(*other.br) : nullptr;

        return *this;
    }
    Quadtree<T>& operator=(Quadtree<T>&& other) = default;

    size_t size() const;
    Area getArea() const { return area; }

    value_type* insert(const value_type& key_elem);
    value_type* insert(value_type&& key_elem);

    void erase(const value_type* key_elem);

    std::vector<value_type*> find(const Vec2& key);
    std::vector<const value_type*> find(const Vec2& key) const;

    std::vector<value_type*> find(const Area& area);
    std::vector<const value_type*> find(const Area& area) const;

    template<typename visit_f>
    void forEach(const Vec2& key, const visit_f& visit);
    template<typename visit_f>
    void forEach(const Vec2& key, const visit_f& visit) const;
    template<typename visit_f>
    void forEach(const Area& area, const visit_f& visit);
    template<typename visit_f>
    void forEach(const Area& area, const visit_f& visit) const;

    template<typename visit_f>
    bool forEachUntil(const Vec2& key, const visit_f& visit);
    template<typename visit_f>
    bool forEachUntil(const Vec2& key, const visit_f& visit) const;
    template<typename visit_f>
    bool forEachUntil(const Area& area, const visit_f& visit);
    template<typename visit_f>
    bool forEachUntil(const Area& area, const visit_f& visit) const;
private:
    template<typename append_f>
    value_type* insert(const Vec2& key, append_f append);
    value_type* insert(typename std::list<value_type>::const_iterator it, std::list<value_type>& src);
};

template<typename T>
void Quadtree<T>::split()
{
    const Vec2 half_size = area.size() / 2;
    const Area tl_area = { area.tl, area.tl + half_size };
    const Area tr_area = { { area.tl.x + half_size.x, area.tl.y }, { area.br.x, area.tl.y + half_size.y } };
    const Area bl_area = { {area.tl.x, area.tl.y + half_size.y},{area.tl.x + half_size.x, area.br.y} };
    const Area br_area = { area.tl + half_size, area.br };

    tl = std::make_unique<Quadtree<T>>(tl_area);
    tr = std::make_unique<Quadtree<T>>(tr_area);
    bl = std::make_unique<Quadtree<T>>(bl_area);
    br = std::make_unique<Quadtree<T>>(br_area);

    while (!elements.empty())
    {
        auto elem = elements.begin();
        if (tl->area.contains(elem->first)) tl->insert(elem, elements);
        else if (tr->area.contains(elem->first)) tr->insert(elem, elements);
        else if (bl->area.contains(elem->first)) bl->insert(elem, elements);
        else if (br->area.contains(elem->first)) br->insert(elem, elements);
    }
}

template<typename T>
size_t Quadtree<T>::size() const
{
    if (isLeaf())
    {
        return elements.size();
    }
    return (tl ? tl->size() : 0)
        + (tr ? tr->size() : 0)
        + (bl ? bl->size() : 0)
        + (br ? br->size() : 0);
}

template<typename T>
bool Quadtree<T>::isLeaf() const
{
    return !(tl || tr || bl || br);
}

template<typename T>
std::list<typename Quadtree<T>::value_type> Quadtree<T>::get()
{
    std::list<value_type> el;

    if (isLeaf())
    {
        el.splice(el.begin(), elements);
        return el;
    }

    el.splice(el.begin(), tl->get());
    el.splice(el.begin(), tr->get());
    el.splice(el.begin(), bl->get());
    el.splice(el.begin(), br->get());

    return el;
}

template<typename T>
auto Quadtree<T>::insert(const value_type& key_elem) -> value_type*
{
    return insert(
        key_elem.first,
        [&](auto& elements)
        {
            return &*elements.insert(elements.begin(), key_elem);
        });
}

template<typename T>
auto Quadtree<T>::insert(value_type&& key_elem) -> value_type*
{
    return insert(
        key_elem.first,
        [&](auto& elements)
        {
            return &*elements.insert(elements.begin(), std::move(key_elem));
        });
}

template<typename T>
auto Quadtree<T>::insert(typename std::list<value_type>::const_iterator it, std::list<value_type>& src) -> value_type*
{
    return insert(
        it->first,
        [&](auto& elements)
        {
            elements.splice(elements.begin(), src, it);
            return &*elements.begin();
        });
}

template<typename T>
template<typename append_f>
auto Quadtree<T>::insert(const Vec2& key, append_f append) -> value_type*
{
    if (!area.contains(key))
    {
        return nullptr;
    }
    if (isLeaf())
    {
        if (elements.size() < max_size || area.size() == Vec2{ 1,1 })
        {
            return append(elements);
        }
        else split();
    }
    if (tl->area.contains(key)) return tl->insert(key, append);
    else if (tr->area.contains(key)) return tr->insert(key, append);
    else if (bl->area.contains(key)) return bl->insert(key, append);
    else if (br->area.contains(key)) return br->insert(key, append);
}


template<typename T>
void Quadtree<T>::erase(const value_type* key_elem)
{
    if (isLeaf())
    {
        for (auto it = elements.begin(); it != elements.end(); it++)
        {
            if (&*it == key_elem)
            {
                elements.erase(it);
                return;
            }
        }
    }
    else
    {
        if (tl->area.contains(key_elem->first)) tl->erase(key_elem);
        else if (tr->area.contains(key_elem->first)) tr->erase(key_elem);
        else if (bl->area.contains(key_elem->first)) bl->erase(key_elem);
        else if (br->area.contains(key_elem->first)) br->erase(key_elem);
        if (size() <= max_size)
        {
            elements.splice(elements.begin(), get());
            tl.reset();
            tr.reset();
            bl.reset();
            br.reset();
        }
    }
}

template<typename T>
auto Quadtree<T>::find(const Vec2& key) const -> std::vector<const value_type*>
{
    std::vector<const value_type*> values;
    forEach(key, [&](const value_type& value) {
        values.push_back(&value);
    });
    return values;
}

template<typename T>
auto Quadtree<T>::find(const Vec2& key) -> std::vector<value_type*>
{
    std::vector<value_type*> values;
    forEach(key, [&](value_type& value) {
        values.push_back(&value);
    });
    return values;
}

template<typename T>
template<typename visit_f>
void Quadtree<T>::forEach(const Vec2& key, const visit_f& visit)
{
    std::as_const(*this).forEach(key, [&](const value_type& value) {
        visit(const_cast<value_type&>(value));
    });
}

template<typename T>
template<typename visit_f>
void Quadtree<T>::forEach(const Vec2& key, const visit_f& visit) const
{
    if (isLeaf())
    {
        for (auto it = elements.begin(); it != elements.end(); it++)
        {
            if (it->first == key) visit(const_cast<const value_type&>(*it));
        }
        return;
    }
    if      (tl->area.contains(key)) std::as_const(*tl).forEach(key, visit);
    else if (tr->area.contains(key)) std::as_const(*tr).forEach(key, visit);
    else if (bl->area.contains(key)) std::as_const(*bl).forEach(key, visit);
    else if (br->area.contains(key)) std::as_const(*br).forEach(key, visit);
}

template<typename T>
template<typename visit_f>
bool Quadtree<T>::forEachUntil(const Vec2& key, const visit_f& visit)
{
    return std::as_const(*this).forEachUntil(key, [&](const value_type& value) {
        return visit(const_cast<value_type&>(value));
    });
}

template<typename T>
template<typename visit_f>
bool Quadtree<T>::forEachUntil(const Vec2& key, const visit_f& visit) const
{
    if (isLeaf())
    {
        for (auto it = elements.begin(); it != elements.end(); it++)
        {
            if (it->first == key && visit(const_cast<const value_type&>(*it))) return true;
        }
        return false;
    }
    if      (tl->area.contains(key) && std::as_const(*tl).forEachUntil(key, visit)) return true;
    else if (tr->area.contains(key) && std::as_const(*tr).forEachUntil(key, visit)) return true;
    else if (bl->area.contains(key) && std::as_const(*bl).forEachUntil(key, visit)) return true;
    else if (br->area.contains(key) && std::as_const(*br).forEachUntil(key, visit)) return true;

    return false;
}

template<typename T>
auto Quadtree<T>::find(const Area& a) const -> std::vector<const value_type*>
{
    std::vector<const value_type*> values;
    forEach(a, [&](const value_type& value) {
        values.push_back(&value);
    });
    return values;
}

template<typename T>
auto Quadtree<T>::find(const Area& a) -> std::vector<value_type*>
{
    std::vector<value_type*> values;
    forEach(a, [&](value_type& value) {
        values.push_back(&value);
    });
    return values;
}

template<typename T>
template<typename visit_f>
void Quadtree<T>::forEach(const Area& area, const visit_f& visit)
{
    std::as_const(*this).forEach(area, [&](const value_type& value) {
        visit(const_cast<value_type&>(value));
    });
}

template<typename T>
template<typename visit_f>
void Quadtree<T>::forEach(const Area& a, const visit_f& visit) const
{
    if (isLeaf())
    {
        for (auto it = elements.begin(); it != elements.end(); it++)
        {
            if (a.contains(it->first)) visit(const_cast<const value_type&>(*it));
        }
        return;
    }

    if (tl->area.intersects(a)) std::as_const(*tl).forEach(a, visit);
    if (tr->area.intersects(a)) std::as_const(*tr).forEach(a, visit);
    if (bl->area.intersects(a)) std::as_const(*bl).forEach(a, visit);
    if (br->area.intersects(a)) std::as_const(*br).forEach(a, visit);
}

template<typename T>
template<typename visit_f>
bool Quadtree<T>::forEachUntil(const Area& area, const visit_f& visit)
{
    return std::as_const(*this).forEachUntil(area, [&](const value_type& value) {
        return visit(const_cast<value_type&>(value));
    });
}

template<typename T>
template<typename visit_f>
bool Quadtree<T>::forEachUntil(const Area& a, const visit_f& visit) const
{
    if (isLeaf())
    {
        for (auto it = elements.begin(); it != elements.end(); it++)
        {
            if (a.contains(it->first) && visit(const_cast<const value_type&>(*it))) return true;
        }
        return false;
    }

    if (tl->area.intersects(a) && std::as_const(*tl).forEachUntil(a, visit)) return true;
    if (tr->area.intersects(a) && std::as_const(*tr).forEachUntil(a, visit)) return true;
    if (bl->area.intersects(a) && std::as_const(*bl).forEachUntil(a, visit)) return true;
    if (br->area.intersects(a) && std::as_const(*br).forEachUntil(a, visit)) return true;

    return false;
}
