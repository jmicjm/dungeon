#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

#include <chrono>
#include <memory>
#include <vector>


class animated_sprite : public sf::Drawable
{
	std::shared_ptr<std::vector<sf::Sprite>> m_frames;
	unsigned int m_current_frame_idx = 0;

	sf::Vector2f m_position = {0,0};
	sf::Vector2f m_origin = {0,0};
	sf::Vector2f m_scale = {1,1};
	float m_rotation = 0;
	sf::Color m_color = {255,255,255,255};

	std::chrono::milliseconds m_frame_time = std::chrono::milliseconds(1000)/30;
	std::chrono::steady_clock::time_point m_frame_start_point;

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
	void move(sf::Vector2f offset);
	sf::Vector2f getPosition() const;

	void setScale(sf::Vector2f scale);
	void scale(sf::Vector2f scale);
	sf::Vector2f getScale() const;

	void setRotation(float angle);
	void rotate(float angle);
	float getRotation() const;

	void setOrigin(sf::Vector2f origin);
	sf::Vector2f getOrigin() const;

	void setColor(sf::Color color);
	sf::Color getColor() const;

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;
};