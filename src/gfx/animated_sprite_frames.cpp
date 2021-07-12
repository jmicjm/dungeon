#include "animated_sprite_frames.h"
#include "utils/setQuad.h"

Animated_sprite_frames::Animated_sprite_frames(
	const sf::Texture* texture,
	const std::vector<sf::IntRect>& rects) : texture(texture)
{
	frame_rects.resize(rects.size());
	for (int i = 0; i < frame_rects.size(); i++)
	{
		setQuad(frame_rects[i].vertices, rects[i]);
	}
}