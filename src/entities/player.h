#pragma once
#include "entity.h"
#include "../gfx/animated_sprite.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Drawable.hpp"

class player : public entity
{
	animated_sprite animation;

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

	void setPosition(const sf::Vector2i& position) override;
	void move(sf::Vector2i& offset) override;

	void updateAnimationPosition();
public:
	player(level* lvl, sf::Vector2i position, animated_sprite anim);

	void updateState(const bool make_action) override;
};