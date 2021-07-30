#include "player.h"

#include "SFML/Window/Keyboard.hpp"


void Player::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(animation, st);
}

void Player::setPosition(const sf::Vector2i& position)
{
    Entity::setPosition(position);
    updateAnimationPosition();
}

void Player::move(sf::Vector2i& offset)
{
    Entity::move(offset);
    updateAnimationPosition();
}

void Player::updateAnimationPosition()
{
    const auto [px, py] = Entity::getPosition();
    const auto [tx, ty] = m_level->tile_size;
    const sf::Vector2f anim_pos(px * tx, py * ty - ty/2);

    animation.setPosition(anim_pos);
}

Player::Player(Level* lvl, sf::Vector2i position, Animated_sprite anim) : Entity(lvl, position), animation(anim) 
{
    updateAnimationPosition();
}

void Player::updateState(const bool make_action)
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