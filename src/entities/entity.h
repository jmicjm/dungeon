#pragma once
#include "../level/level.h"
#include <SFML/Graphics/Drawable.hpp>

class entity : public sf::Drawable
{
private:
	sf::Vector2i m_position;
	level* m_level = nullptr;

public:
	entity(level* lvl, const sf::Vector2i& position);
	virtual ~entity() = default;

	virtual void setPosition(const sf::Vector2i& position);
	sf::Vector2i getPosition() const;

	virtual void move(sf::Vector2i& offset);

	virtual void updateState(const bool make_action);
};