#include "player.h"
#include "../world/world.h"
#include "../level/level.h"

#include "SFML/Window/Keyboard.hpp"


void Player::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(animation, st);
}

Player::Player(Level* level, sf::Vector2i position, Animated_sprite anim)
    : Entity(level, position), animation(anim) {}

void Player::update()
{
    animation.updateFrameIdx();
}

bool Player::performAction()
{
    std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
    if ((t - last_action_t) < std::chrono::milliseconds(200)) return false;
    last_action_t = t;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        auto entrances = level->entrances.find(getPosition());
        if (entrances.size() > 0)
        {
            if (auto destination = entrances[0]->second.getDestination().lock(); destination)
            {
                useEntrance(entrances[0]->second);
                world->current_level = destination;
                return true;
            }
        }
    }

    sf::Vector2i offset = { 0,0 };

    offset.x += sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    offset.x -= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    offset.y += sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    offset.y -= sf::Keyboard::isKeyPressed(sf::Keyboard::W);

    const sf::Vector2i old_pos = getPosition();
    move(offset);

    return old_pos != getPosition();
}