#include "position.h"


Position::Position(const sf::Vector2i& coords, Level* level, std::unordered_map<const Level*, Quadtree<entt::entity>>& elvl_map, entt::entity entity)
  : coords(coords),
    level(level),
    qt_ptr(elvl_map[level].insert({ coords, entity })),
    elvl_map(&elvl_map) {}

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

void Position::setLevel(Level* level)
{
    auto entity = qt_ptr->second;
    (*elvl_map)[this->level].erase(qt_ptr);
    this->level = level;
    qt_ptr = (*elvl_map)[this->level].insert({coords, entity});
}

Level* Position::getLevel() const
{
    return level;
}
