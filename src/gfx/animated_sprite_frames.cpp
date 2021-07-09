#include "animated_sprite_frames.h"

#include <iostream>

animated_sprite_frames::animated_sprite_frames(
	const sf::Texture* texture,
	const std::vector<sf::IntRect>& rects) : texture(texture)
{
	frame_rects.resize(rects.size());
	for (int i = 0; i < frame_rects.size(); i++)
	{
		const float left   = rects[i].left;
		const float right  = left + rects[i].width;
		const float top    = rects[i].top;
		const float bottom = top + rects[i].height;

		frame_rects[i].vertices[0].texCoords = sf::Vector2f{ left,  top    };
		frame_rects[i].vertices[1].texCoords = sf::Vector2f{ left,  bottom };
		frame_rects[i].vertices[2].texCoords = sf::Vector2f{ right, bottom };
		frame_rects[i].vertices[3].texCoords = sf::Vector2f{ right, top    };

		const float& width  = rects[i].width;
		const float& height = rects[i].height;

		frame_rects[i].vertices[0].position = sf::Vector2f{ 0,     0      };
		frame_rects[i].vertices[1].position = sf::Vector2f{ 0,     height };
		frame_rects[i].vertices[2].position = sf::Vector2f{ width, height };
		frame_rects[i].vertices[3].position = sf::Vector2f{ width, 0      };
	}
}