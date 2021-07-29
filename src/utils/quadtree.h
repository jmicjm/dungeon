#pragma once
#include <vector>
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
            return point.x >= tl.x && point.x < br.x && point.y >= tl.y && point.y < br.y;
        }
        bool intersects(const Area& other) const
        {
            return tl.x <= other.br.x && br.x > other.tl.x
                && tl.y <= other.br.y && br.y > other.tl.y;
        };
    } area;

    constexpr static size_t max_size = 16;
    using value_type = std::pair<Vec2, T>;

    std::vector<value_type> elements; 

    std::unique_ptr<Quadtree<T>> tl;
    std::unique_ptr<Quadtree<T>> tr;
    std::unique_ptr<Quadtree<T>> bl;
    std::unique_ptr<Quadtree<T>> br;

    void split();
    bool isLeaf() const;
    std::vector<value_type> get() const;

public:
    Quadtree(const Area& area) 
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
    }

    Quadtree<T>& operator=(Quadtree<T>&& other)
    {
        area = other.area;
        elements =std::move(other.elements);
        tl = std::move(other.tl);
        tr = std::move(other.tr);
        bl = std::move(other.bl);
        br = std::move(other.br);
    }

    size_t size() const;
    Area getArea() const { return area; }

    void insert(const value_type& key_elem);
    void erase (const value_type& key_elem);
    std::vector<value_type> find(const Vec2& key ) const;
    std::vector<value_type> find(const Area& area) const;
};

template<typename T>
inline void Quadtree<T>::split()
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

    for (const auto& elem : elements)
    {
        if      (tl->area.contains(elem.first)) tl->insert(elem);
        else if (tr->area.contains(elem.first)) tr->insert(elem);
        else if (bl->area.contains(elem.first)) bl->insert(elem);
        else if (br->area.contains(elem.first)) br->insert(elem);
    }
    elements.clear();
}

template<typename T>
inline size_t Quadtree<T>::size() const
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
inline bool Quadtree<T>::isLeaf() const
{
    return !(tl || tr || bl || br);
}

template<typename T>
inline std::vector<typename Quadtree<T>::value_type> Quadtree<T>::get() const
{
    if (isLeaf())
    {     
        return elements;
    }
    auto combine = [](auto& a, const auto& b)
    {
        a.insert(a.end(), b.begin(), b.end());
    };

    std::vector<value_type> el;
    combine(el, tl->get());
    combine(el, tr->get());
    combine(el, bl->get());
    combine(el, br->get());

    return el;
}

template<typename T>
inline void Quadtree<T>::insert(const value_type& key_elem)
{
    if (!area.contains(key_elem.first))
    {
        return;
    }
    if (isLeaf())
    {
        if (elements.size() < max_size || area.size() == Vec2{1,1})
        {
            elements.push_back(key_elem);
            return;
        }
        split();
    }
    if      (tl->area.contains(key_elem.first)) tl->insert(key_elem);
    else if (tr->area.contains(key_elem.first)) tr->insert(key_elem);
    else if (bl->area.contains(key_elem.first)) bl->insert(key_elem);
    else if (br->area.contains(key_elem.first)) br->insert(key_elem);
}

template<typename T>
inline void Quadtree<T>::erase(const value_type& key_elem)
{
    if (isLeaf())
    {
        for (int i = elements.size() - 1; i >= 0; i--)
        {
            if (elements[i] == key_elem)
            {
                elements.erase(elements.begin() + i);
            }
        }
    }
    else
    {
        if      (tl->area.contains(key_elem.first)) tl->erase(key_elem);
        else if (tr->area.contains(key_elem.first)) tr->erase(key_elem);
        else if (bl->area.contains(key_elem.first)) bl->erase(key_elem);
        else if (br->area.contains(key_elem.first)) br->erase(key_elem);
        if (size() <= max_size)
        {
            elements = get();
            tl.release();
            tr.release();
            bl.release();
            br.release();
        }
    }
}

template<typename T>
inline std::vector<typename Quadtree<T>::value_type> Quadtree<T>::find(const Vec2& key) const
{
    if (isLeaf())
    {
        std::vector<value_type> el;
        for (const auto& elem : elements)
        {
            if (elem.first == key) el.push_back(elem);
        }       
        return el;
    }
    if (tl->area.contains(key)) return tl->find(key);
    if (tr->area.contains(key)) return tr->find(key);
    if (bl->area.contains(key)) return bl->find(key);
    if (br->area.contains(key)) return br->find(key);

    return std::vector<value_type>();
}

template<typename T>
inline std::vector<typename Quadtree<T>::value_type> Quadtree<T>::find(const Area& a) const
{
    std::vector<value_type> el;

    if (isLeaf())
    {
        for (const auto& elem : elements)
        {
            if (a.contains(elem.first)) el.push_back(elem);
        }
        return el;
    }
    auto combine = [](auto& a, const auto& b)
    {
        a.insert(a.end(), b.begin(), b.end());
    };

    if (tl->area.intersects(a)) combine(el, tl->find(a));
    if (tr->area.intersects(a)) combine(el, tr->find(a));
    if (bl->area.intersects(a)) combine(el, bl->find(a));
    if (br->area.intersects(a)) combine(el, br->find(a));

    return el;
}