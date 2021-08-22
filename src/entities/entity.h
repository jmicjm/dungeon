#pragma once
#include "../level/level.h"
#include "../level/tile_visibility_info.h"
#include "../utils/sf_vector2_hash.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML//Graphics/RenderWindow.hpp"

#include <utility>
#include <unordered_map>

class Entity : public sf::Drawable
{
private:
    sf::Vector2i m_position;

protected:
    Level* m_level = nullptr;

    unsigned int m_vision_radius = 6;

public:
    Entity(Level* level, const sf::Vector2i& position);
    virtual ~Entity() = default;

    virtual void setPosition(const sf::Vector2i& position);
    sf::Vector2i getPosition() const;

    virtual void move(sf::Vector2i& offset);

    virtual void updateState(const bool make_action);

    std::unordered_map<sf::Vector2i, Tile_visibility_info> getVisibleTiles() const;
};