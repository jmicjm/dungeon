#include "animated_sprite.h"

animated_sprite::animated_sprite(std::shared_ptr<animated_sprite_frames> frames, unsigned int fps)
	: animated_sprite(frames, std::chrono::milliseconds(1000) / fps) {}

animated_sprite::animated_sprite(std::shared_ptr<animated_sprite_frames> frames, std::chrono::milliseconds frame_time)
	: m_frames(frames), m_frame_time(frame_time)
{
	restart();
}

void animated_sprite::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
	if (m_frames && m_current_frame_idx < m_frames->frame_rects.size())
	{
		auto& vertices = m_frames->frame_rects[m_current_frame_idx].vertices;

		for (auto& i : vertices)
		{
			i.color = m_color;
		}

		st.transform *= getTransform();
		st.texture = m_frames->texture;

		rt.draw(vertices, 4, sf::Quads, st);
	}
}

void animated_sprite::restart()
{
	restart(std::chrono::steady_clock::now());
}

void animated_sprite::restart(std::chrono::steady_clock::time_point tp)
{
	m_frame_start_point = tp;
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

void animated_sprite::setFrameIdx(unsigned int idx, std::chrono::steady_clock::time_point tp)
{
	m_current_frame_idx = std::min(idx, frameCount() - 1);
	m_frame_start_point = tp;
}

unsigned int animated_sprite::getFrameIdx() const
{
	return m_current_frame_idx;
}

unsigned int animated_sprite::frameCount() const
{
	return m_frames ? m_frames->frame_rects.size() : 0;
}

void animated_sprite::updateFrameIdx()
{
	updateFrameIdx(std::chrono::steady_clock::now());
}

void animated_sprite::updateFrameIdx(std::chrono::steady_clock::time_point tp)
{
	if (m_frames && m_frames->frame_rects.size() > 1)
	{
		std::chrono::steady_clock::time_point current_point = tp;

		unsigned int increase = (current_point - m_frame_start_point) / m_frame_time;

		if (increase > 0)
		{
			m_current_frame_idx = (m_current_frame_idx + increase) % m_frames->frame_rects.size();
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
	if (m_frames && m_current_frame_idx < m_frames->frame_rects.size())
	{
		auto& vertices     = m_frames->frame_rects[m_current_frame_idx].vertices;
		const float width  = vertices[3].position.x;
		const float height = vertices[1].position.y;

		return sf::FloatRect{ 0,0, width, height };
	}
	return sf::FloatRect{0,0,0,0};
}

sf::FloatRect animated_sprite::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}