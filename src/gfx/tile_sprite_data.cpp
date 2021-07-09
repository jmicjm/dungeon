#include "tile_sprite_data.h"
#include "utils/setQuad.h"

tile_sprite_data::tile_sprite_data(const sf::Texture* texture, const sf::IntRect& rect) : texture(texture)
{
	setQuad(vertices, rect);
}