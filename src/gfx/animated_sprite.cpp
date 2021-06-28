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
		(*m_frames)[m_current_frame_idx].setColor(m_color);

		st.transform *= getTransform();
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
