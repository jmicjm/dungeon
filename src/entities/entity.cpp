#include "entity.h"
#include <algorithm>

entity::entity(level* lvl, const sf::Vector2i& position) : m_level(lvl)
{
	setPosition(position);
}

void entity::setPosition(const sf::Vector2i& position)
{
	//todo: if position is on unreachable tile(wall, closed door, occupied tile) find closest reachable tile
	m_position = position;
}

sf::Vector2i entity::getPosition() const
{
	return m_position;
}

void entity::move(sf::Vector2i& offset)
{
	offset.x = std::clamp(offset.x, -1, 1);
	offset.y = std::clamp(offset.y, -1, 1);

	const sf::Vector2i new_pos = m_position + offset;

	const sf::Vector2i new_pos_x = m_position + sf::Vector2i{ offset.x, 0 };
	const sf::Vector2i new_pos_y = m_position + sf::Vector2i{ 0, offset.y };

	if (!( m_level->ls.at({ new_pos_x.x, new_pos_x.y }).type == TILE_TYPE::WALL && m_level->ls.at({ new_pos_y.x, new_pos_y.y }).type == TILE_TYPE::WALL)
		&& m_level->ls.at({ new_pos.x, new_pos.y }).type != TILE_TYPE::WALL)
	{
		m_position = new_pos;
	}
}

void entity::updateState(const bool make_action) {}