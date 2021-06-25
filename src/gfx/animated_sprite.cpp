#include "animated_sprite.h"

animated_sprite::animated_sprite(std::shared_ptr<std::vector<sf::Sprite>> frames, unsigned int fps)
	: animated_sprite(frames, std::chrono::milliseconds(1000) / fps) {}

animated_sprite::animated_sprite(std::shared_ptr<std::vector<sf::Sprite>> frames, std::chrono::milliseconds frame_time)
	: m_frames(frames), m_frame_time(frame_time)
{
	restart();
}

void animated_sprite::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
	if (m_frames && m_current_frame_idx < m_frames->size())
	{
		(*m_frames)[m_current_frame_idx].setPosition(m_position);
		(*m_frames)[m_current_frame_idx].setScale(m_scale);
		(*m_frames)[m_current_frame_idx].setOrigin(m_origin);
		(*m_frames)[m_current_frame_idx].setRotation(m_rotation);
		(*m_frames)[m_current_frame_idx].setColor(m_color);

		rt.draw((*m_frames)[m_current_frame_idx], st);
	}
}

void animated_sprite::restart()
{
	m_frame_start_point = std::chrono::steady_clock::now();
	m_current_frame_idx = 0;
}

void animated_sprite::setFrameIdx(unsigned int idx, bool update_start_point)
{
	m_current_frame_idx = std::min(idx, frameCount()-1);
	if (update_start_point)
	{
		m_frame_start_point = std::chrono::steady_clock::now();
	}
}

unsigned int animated_sprite::getFrameIdx() const
{
	m_current_frame_idx;
}

unsigned int animated_sprite::frameCount() const
{
	return m_frames ? m_frames->size() : 0;
}

void animated_sprite::updateFrameIdx()
{
	if (m_frames && m_frames->size() > 1)
	{
		std::chrono::steady_clock::time_point current_point = std::chrono::steady_clock::now();

		unsigned int increase = (current_point - m_frame_start_point) / m_frame_time;

		if (increase > 0)
		{
			m_current_frame_idx = (m_current_frame_idx + increase) % m_frames->size();
			m_frame_start_point += m_frame_time * increase;
		}
	}
	else
	{
		m_current_frame_idx = 0;
	}
}

void animated_sprite::setPosition(sf::Vector2f pos)
{
	m_position = pos;
}

void animated_sprite::move(sf::Vector2f offset)
{
	m_position += offset;
}

sf::Vector2f animated_sprite::getPosition() const
{
	return m_position;
}

void animated_sprite::setScale(sf::Vector2f scale)
{
	m_scale = scale;
}

void animated_sprite::scale(sf::Vector2f scale)
{
	m_scale = { m_scale.x * scale.x, m_scale.y * scale.y };
}

sf::Vector2f animated_sprite::getScale() const
{
	return m_scale;
}

void animated_sprite::setRotation(float angle)
{
	m_rotation = angle;
}

void animated_sprite::rotate(float angle)
{
	m_rotation += angle;
}

float animated_sprite::getRotation() const
{
	return m_rotation;
}

void animated_sprite::setOrigin(sf::Vector2f origin)
{
	m_origin = origin;
}

sf::Vector2f animated_sprite::getOrigin() const
{
	return m_origin;
}

void animated_sprite::setColor(sf::Color color)
{
	m_color = color;
}

sf::Color animated_sprite::getColor() const
{
	return m_color;
}

sf::FloatRect animated_sprite::getLocalBounds() const
{
	return (m_frames && m_current_frame_idx < m_frames->size()) 
		 ? (*m_frames)[m_current_frame_idx].getLocalBounds() 
		 : sf::FloatRect{0,0,0,0};
}

sf::FloatRect animated_sprite::getGlobalBounds() const
{
	return (m_frames && m_current_frame_idx < m_frames->size()) 
		 ? (*m_frames)[m_current_frame_idx].getGlobalBounds() 
		 : sf::FloatRect{ 0,0,0,0 };
}
