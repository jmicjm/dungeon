#include "player.h"

#include "SFML/Window/Keyboard.hpp"


void Player::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(animation, st);
}

Player::Player(Level* lvl, sf::Vector2i position, Animated_sprite anim) 
    : Entity(lvl, position), animation(anim) {}

void Player::update()
{
    animation.updateFrameIdx();
}

bool Player::performAction()
{
    sf::Vector2i offset = { 0,0 };

    offset.x += sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    offset.x -= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    offset.y += sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    offset.y -= sf::Keyboard::isKeyPressed(sf::Keyboard::W);

    const sf::Vector2i old_pos = getPosition();
    move(offset);

    return old_pos != getPosition();
}