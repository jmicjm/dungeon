#pragma once
#include "animated_sprite_frames.h"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <chrono>
#include <memory>
#include <vector>


class animated_sprite : public sf::Drawable, public sf::Transformable
{
	std::shared_ptr<animated_sprite_frames> m_frames;
	unsigned int m_current_frame_idx = 0;

	sf::Color m_color = {255,255,255,255};

	std::chrono::milliseconds m_frame_time = std::chrono::milliseconds(1000)/30;
	std::chrono::steady_clock::time_point m_frame_start_point;

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
	
public:
	animated_sprite(std::shared_ptr<animated_sprite_frames> frames, unsigned int fps);
	animated_sprite(std::shared_ptr<animated_sprite_frames> frames, std::chrono::milliseconds frame_time);

	void restart();

	void updateFrameIdx();
	void setFrameIdx(unsigned int idx, bool update_start_point = false);
	unsigned int getFrameIdx() const;
	unsigned int frameCount() const;

	void setColor(sf::Color color);
	sf::Color getColor() const;

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
};