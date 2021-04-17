#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <chrono>
#include <memory>
#include <vector>


class animation : public sf::Drawable
{
	std::shared_ptr<std::vector<sf::Sprite>> frames;
	unsigned int current_frame_idx = 0;
	sf::Vector2f position;

	std::chrono::milliseconds frame_time = std::chrono::milliseconds(1000)/30;
	std::chrono::steady_clock::time_point frame_start_point;
	
public:
	animation(std::shared_ptr<std::vector<sf::Sprite>> frames, unsigned int fps) 
		: animation(frames, std::chrono::milliseconds(1000)/fps) {}
	animation(std::shared_ptr<std::vector<sf::Sprite>> frames, std::chrono::milliseconds frame_time)
		: frames(frames), frame_time(frame_time) 
	{
		restart();
	}

	void restart();

	void updateFrameIdx();
	void setFrameIdx(unsigned int idx);
	unsigned int getFrameIdx() const;
	unsigned int getFrameCount() const;

	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition() const;

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
};