#pragma once
#include "../level/level.h"
#include <SFML/Graphics/Drawable.hpp>

class Entity : public sf::Drawable
{
private:
    sf::Vector2i m_position;
    Level* m_Level = nullptr;

public:
    Entity(Level* level, const sf::Vector2i& position);
    virtual ~Entity() = default;

    virtual void setPosition(const sf::Vector2i& position);
    sf::Vector2i getPosition() const;

    virtual void move(sf::Vector2i& offset);

    virtual void updateState(const bool make_action);
};