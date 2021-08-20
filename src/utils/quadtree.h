#pragma once
#include <vector>
#include <list>
#include <memory>

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
    Quadtree(Quadtree<T>&& other)
        : area(other.area),
        elements(std::move(other.elements)),
        tl(std::move(other.tl)),
        tr(std::move(other.tr)),
        bl(std::move(other.bl)),
        br(std::move(other.br)) {}

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

    Quadtree<T>& operator=(Quadtree<T>&& other)
    {
        area = other.area;
        elements = std::move(other.elements);
        tl = std::move(other.tl);
        tr = std::move(other.tr);
        bl = std::move(other.bl);
        br = std::move(other.br);

        return *this;
    }

    size_t size() const;
    Area getArea() const { return area; }

    value_type* insert(const value_type& key_elem);
    void        erase(const value_type* key_elem);
    std::list<const value_type*> find(const Vec2& key) const;
    std::list<value_type*>       find(const Vec2& key);
    std::list<value_type*>       find(const Area& area);
    std::list<const value_type*> find(const Area& area) const;
private:
    void insert(typename std::list<value_type>::const_iterator it, std::list<value_type>& src);
    template<typename value_t, typename subtree_t>
    std::list<value_t> find(const Vec2& key) const;
    template<typename value_t, typename subtree_t>
    std::list<value_t> find(const Area& area) const;
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
typename Quadtree<T>::value_type* Quadtree<T>::insert(const value_type& key_elem)
{
    std::list<value_type> l = { key_elem };
    value_type* i = &*l.begin();
    insert(l.begin(), l);

    return i;
}

template<typename T>
void Quadtree<T>::insert(typename std::list<value_type>::const_iterator it, std::list<value_type>& src)
{
    if (!area.contains(it->first))
    {
        return;
    }
    if (isLeaf())
    {
        if (elements.size() < max_size || area.size() == Vec2{ 1,1 })
        {
            elements.splice(elements.begin(), src, it);
            return;
        }
        split();
    }
    if      (tl->area.contains(it->first)) tl->insert(it, src);
    else if (tr->area.contains(it->first)) tr->insert(it, src);
    else if (bl->area.contains(it->first)) bl->insert(it, src);
    else if (br->area.contains(it->first)) br->insert(it, src);
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
        if      (tl->area.contains(key_elem->first)) tl->erase(key_elem);
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
std::list<const typename Quadtree<T>::value_type*> Quadtree<T>::find(const Vec2& key) const
{
    return find<const value_type*, decltype(*this)>(key);
}

template<typename T>
std::list<typename Quadtree<T>::value_type*> Quadtree<T>::find(const Vec2& key)
{
    return find<value_type*, decltype(*this)>(key);
}

template<typename T>
template<typename value_t, typename subtree_t>
std::list<value_t> Quadtree<T>::find(const Vec2& key) const
{
    if (isLeaf())
    {
        std::list<value_t> el;

        for (auto it = elements.begin(); it != elements.end(); it++)
        {
            if (it->first == key) el.push_back(const_cast<value_t>(&*it));
        }
        return el;
    }
    if (tl->area.contains(key)) return const_cast<subtree_t&>(*tl).find(key);
    if (tr->area.contains(key)) return const_cast<subtree_t&>(*tr).find(key);
    if (bl->area.contains(key)) return const_cast<subtree_t&>(*bl).find(key);
    if (br->area.contains(key)) return const_cast<subtree_t&>(*br).find(key);

    return std::list<value_t>();
}

template<typename T>
std::list<typename Quadtree<T>::value_type*> Quadtree<T>::find(const Area& a)
{
    return find<value_type*, decltype(*this)>(a);
}

template<typename T>
std::list<const typename Quadtree<T>::value_type*> Quadtree<T>::find(const Area& a) const
{
    return find<const value_type*, decltype(*this)>(a);
}

template<typename T>
template<typename value_t, typename subtree_t>
std::list<value_t> Quadtree<T>::find(const Area& a) const
{
    std::list<value_t> el;

    if (isLeaf())
    {
        for (auto it = elements.begin(); it != elements.end(); it++)
        {
            if (a.contains(it->first)) el.push_back(const_cast<value_t>(&*it));
        }
        return el;
    }

    if (tl->area.intersects(a)) el.splice(el.begin(), const_cast<subtree_t&>(*tl).find(a));
    if (tr->area.intersects(a)) el.splice(el.begin(), const_cast<subtree_t&>(*tr).find(a));
    if (bl->area.intersects(a)) el.splice(el.begin(), const_cast<subtree_t&>(*bl).find(a));
    if (br->area.intersects(a)) el.splice(el.begin(), const_cast<subtree_t&>(*br).find(a));

    return el;
}
