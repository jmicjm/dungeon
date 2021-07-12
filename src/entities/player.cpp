#include "player.h"

#include "SFML/Window/Keyboard.hpp"


void player::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
	rt.draw(animation, st);
}

void player::setPosition(const sf::Vector2i& position)
{
	entity::setPosition(position);
	updateAnimationPosition();
}

void player::move(sf::Vector2i& offset)
{
	entity::move(offset);
	updateAnimationPosition();
}

void player::updateAnimationPosition()
{
	animation.setPosition(sf::Vector2f(entity::getPosition()) * 64.f);
}

player::player(level* lvl, sf::Vector2i position, animated_sprite anim) : entity(lvl, position), animation(anim) 
{
	updateAnimationPosition();
}

void player::updateState(const bool make_action)
{
	animation.updateFrameIdx();

	if (make_action)
	{
		sf::Vector2i offset = { 0,0 };

		offset.x += sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		offset.x -= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		offset.y += sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		offset.y -= sf::Keyboard::isKeyPressed(sf::Keyboard::W);

		move(offset);
	}
}