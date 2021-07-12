#pragma once
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/Rect.hpp"

#include <vector>

struct Animated_sprite_frames
{
	const sf::Texture* texture = nullptr;	

	struct frame_rect
	{
		sf::Vertex vertices[4];
	};

	std::vector<frame_rect> frame_rects;

	Animated_sprite_frames(const sf::Texture* texture, const std::vector<sf::IntRect>& rects);
};