#pragma once
#include "entity.h"
#include "../world/world.fwd.h"
#include "../gfx/animated_sprite/animated_sprite.h"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Drawable.hpp"


#include <chrono>

class Player : public Entity
{
    Animated_sprite animation;
    std::chrono::steady_clock::time_point last_action_t = std::chrono::steady_clock::now();

    void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

public:
    World* world = nullptr;
    Player(Level* level, sf::Vector2i position, Animated_sprite anim);

    void update() override;
    bool performAction() override;
};