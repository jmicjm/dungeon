#include "animation.h"


void animation::restart()
{
	frame_start_point = std::chrono::steady_clock::now();
	current_frame_idx = 0;
}

void animation::setFrameIdx(unsigned int idx, bool update_start_point)
{
	current_frame_idx = std::min(idx, getFrameCount()-1);
	if (update_start_point)
	{
		frame_start_point = std::chrono::steady_clock::now();
	}
}

unsigned int animation::getFrameIdx() const
{
	current_frame_idx;
}

unsigned int animation::getFrameCount() const
{
	return frames ? frames->size() : 0;
}

void animation::updateFrameIdx()
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

void animation::setPosition(sf::Vector2f pos)
{
	position = pos;
}

sf::Vector2f animation::getPosition() const
{
	return position;
}

void animation::setScale(sf::Vector2f scl)
{
	scale = scl;
}

sf::Vector2f animation::getScale() const
{
	return scale;
}

void animation::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
	if (frames && current_frame_idx < frames->size())
	{
		(*frames)[current_frame_idx].setPosition(position);
		(*frames)[current_frame_idx].setScale(scale);
		rt.draw((*frames)[current_frame_idx], st);
	}
}