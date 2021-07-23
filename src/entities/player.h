#pragma once
#include "entity.h"
#include "../gfx/animated_sprite/animated_sprite.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Drawable.hpp"

class Player : public Entity
{
    Animated_sprite animation;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

    void setPosition(const sf::Vector2i& position) override;
    void move(sf::Vector2i& offset) override;

    void updateAnimationPosition();
public:
    Player(Level* lvl, sf::Vector2i position, Animated_sprite anim);

    void updateState(const bool make_action) override;
};