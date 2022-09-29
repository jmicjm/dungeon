#include "position.h"


Position::Position(const sf::Vector2i& coords, Level* level, std::unordered_map<const Level*, Quadtree<entt::entity>>& elvl_map, entt::entity entity)
  : coords(coords),
    level(level),
    qt_ptr(elvl_map[level].insert({ coords, entity })),
    elvl_map(&elvl_map) {}

Position::Position(Position&& other) noexcept
  : coords(std::exchange(other.coords, {0,0})),
    level(std::exchange(other.level, nullptr)),
    qt_ptr(std::exchange(other.qt_ptr, nullptr)),
    elvl_map(std::exchange(other.elvl_map, nullptr)) {}

Position& Position::operator=(Position&& other) noexcept
{
    coords = std::exchange(other.coords, { 0,0 });
    level = std::exchange(other.level, nullptr);
    qt_ptr = std::exchange(other.qt_ptr, nullptr);
    elvl_map = std::exchange(other.elvl_map, nullptr);

    return *this;
}

Position::~Position()
{
    if(elvl_map && qt_ptr) (*elvl_map)[this->level].erase(qt_ptr);
}

void Position::setPosition(const sf::Vector2i& coords, Level* level)
{
    auto entity = qt_ptr->second;
    (*elvl_map)[this->level].erase(qt_ptr);
    this->coords = coords;
    this->level = level;
    qt_ptr = (*elvl_map)[this->level].insert({ coords, entity });
}

void Position::setCoords(const sf::Vector2i& coords)
{
    this->coords = coords;
    auto entity = qt_ptr->second;
    (*elvl_map)[level].erase(qt_ptr);
    qt_ptr = (*elvl_map)[level].insert({coords, entity});
}

const sf::Vector2i& Position::getCoords() const
{
    return coords;
}

Level* Position::getLevel() const
{
    return level;
}

Position Position::replicate(entt::entity other) const
{
    return { coords, level, *elvl_map, other };
}
