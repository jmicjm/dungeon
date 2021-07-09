#include "tile_sprite_data.h"

tile_sprite_data::tile_sprite_data(const sf::Texture* texture, const sf::IntRect& rect) : texture(texture)
{
	const float left   = rect.left;
	const float right  = left + rect.width;
	const float top    = rect.top;
	const float bottom = top + rect.height;

	vertices[0].texCoords = sf::Vector2f{ left,  top    };
	vertices[1].texCoords = sf::Vector2f{ left,  bottom };
	vertices[2].texCoords = sf::Vector2f{ right, bottom };
	vertices[3].texCoords = sf::Vector2f{ right, top    };

	const float& width  = rect.width;
	const float& height = rect.height;

	vertices[0].position = sf::Vector2f{ 0,     0      };
	vertices[1].position = sf::Vector2f{ 0,     height };
	vertices[2].position = sf::Vector2f{ width, height };
	vertices[3].position = sf::Vector2f{ width, 0      };
}