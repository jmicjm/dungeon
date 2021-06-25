#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <chrono>
#include <memory>
#include <vector>


class animated_sprite : public sf::Drawable
{
	std::shared_ptr<std::vector<sf::Sprite>> frames;
	unsigned int current_frame_idx = 0;
	sf::Vector2f position;
	sf::Vector2f scale;

	std::chrono::milliseconds frame_time = std::chrono::milliseconds(1000)/30;
	std::chrono::steady_clock::time_point frame_start_point;

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;
	
public:
	animated_sprite(std::shared_ptr<std::vector<sf::Sprite>> frames, unsigned int fps);
	animated_sprite(std::shared_ptr<std::vector<sf::Sprite>> frames, std::chrono::milliseconds frame_time);

	void restart();

	void updateFrameIdx();
	void setFrameIdx(unsigned int idx, bool update_start_point = false);
	unsigned int getFrameIdx() const;
	unsigned int frameCount() const;

	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition() const;
	void setScale(sf::Vector2f scale);
	sf::Vector2f getScale() const;
};