#include "animated_sprite.h"

animated_sprite::animated_sprite(std::shared_ptr<std::vector<sf::Sprite>> frames, unsigned int fps)
	: animated_sprite(frames, std::chrono::milliseconds(1000) / fps) {}

animated_sprite::animated_sprite(std::shared_ptr<std::vector<sf::Sprite>> frames, std::chrono::milliseconds frame_time)
	: frames(frames), frame_time(frame_time)
{
	restart();
}

void animated_sprite::restart()
{
	frame_start_point = std::chrono::steady_clock::now();
	current_frame_idx = 0;
}

void animated_sprite::setFrameIdx(unsigned int idx, bool update_start_point)
{
	current_frame_idx = std::min(idx, frameCount()-1);
	if (update_start_point)
	{
		frame_start_point = std::chrono::steady_clock::now();
	}
}

unsigned int animated_sprite::getFrameIdx() const
{
	current_frame_idx;
}

unsigned int animated_sprite::frameCount() const
{
	return frames ? frames->size() : 0;
}

void animated_sprite::updateFrameIdx()
{
	if (frames && frames->size() > 1)
	{
		std::chrono::steady_clock::time_point current_point = std::chrono::steady_clock::now();

		unsigned int increase = (current_point - frame_start_point) / frame_time;

		if (increase > 0)
		{
			current_frame_idx = (current_frame_idx + increase) % frames->size();
			frame_start_point += frame_time * increase;
		}
	}
	else
	{
		current_frame_idx = 0;
	}
}

void animated_sprite::setPosition(sf::Vector2f pos)
{
	position = pos;
}

sf::Vector2f animated_sprite::getPosition() const
{
	return position;
}

void animated_sprite::setScale(sf::Vector2f scl)
{
	scale = scl;
}

sf::Vector2f animated_sprite::getScale() const
{
	return scale;
}

void animated_sprite::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
	if (frames && current_frame_idx < frames->size())
	{
		(*frames)[current_frame_idx].setPosition(position);
		(*frames)[current_frame_idx].setScale(scale);
		rt.draw((*frames)[current_frame_idx], st);
	}
}