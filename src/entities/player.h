#pragma once
#include "entity.h"
#include "../gfx/animated_sprite/animated_sprite.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Drawable.hpp"

class Player : public Entity
{
    Animated_sprite animation;

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    Player(Level* lvl, sf::Vector2i position, Animated_sprite anim);

    void update() override;
    bool performAction() override;
};