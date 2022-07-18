#include "door.h"
#include "../components/door.h"
#include "../components/gate.h"
#include "../components/nonpassable.h"
#include "../components/opaque.h"
#include "../components/render_component.h"
#include "../asset_storage/texture_bank.h"
#include "../gfx/zlevels.h"


entt::entity createDoorFront(entt::registry& registry)
{
    static auto open_frames = []
    {
        const sf::Texture* tex = Texture_bank::getTexture("assets/textures/tiles/Door_front_open.png");
        std::vector<sf::IntRect> rects = { { {0,0}, {68,74} } };
        return std::make_shared<Animated_sprite_frames>(tex, rects);
    }();
    auto open_rc = Animated_sprite{ open_frames, 1 };
    open_rc.setPosition(sf::Vector2f{ -2, -20 });

    static auto closed_frames = []
    {
        const sf::Texture* tex = Texture_bank::getTexture("assets/textures/tiles/Door_front_closed.png");
        std::vector<sf::IntRect> rects = { { {0,0}, {68,64} } };
        return std::make_shared<Animated_sprite_frames>(tex, rects);
    }();
    auto closed_rc = Animated_sprite{ closed_frames, 1 };
    closed_rc.setPosition(sf::Vector2f{ -2, -10 });

    return createDoor(registry, open_rc, closed_rc);
}

entt::entity createDoorSide(entt::registry& registry)
{
    static auto open_frames = []
    {
        const sf::Texture* tex = Texture_bank::getTexture("assets/textures/tiles/Door_side_open.png");
        std::vector<sf::IntRect> rects = { { {0,0}, {69,100} } };
        return std::make_shared<Animated_sprite_frames>(tex, rects);
    }();
    auto open_rc = Animated_sprite{ open_frames, 1 };
    open_rc.setPosition(sf::Vector2f{ 1, -34 });

    static auto closed_frames = []
    {
        const sf::Texture* tex = Texture_bank::getTexture("assets/textures/tiles/Door_side_closed.png");
        std::vector<sf::IntRect> rects = { { {0,0}, {64,100} } };
        return std::make_shared<Animated_sprite_frames>(tex, rects);
    }();
    auto closed_rc = Animated_sprite{ closed_frames, 1 };
    closed_rc.setPosition(sf::Vector2f{ 1, -34 });

    return createDoor(registry, open_rc, closed_rc);
}

entt::entity createDoor(entt::registry& registry, Animated_sprite open_rc, Animated_sprite closed_rc)
{
    Render_component open{ { { zlevel::door, { open_rc } } } };
    Render_component closed{ { { zlevel::door, { closed_rc } } } };

    auto entity = registry.create();
    registry.emplace<Door>(entity);
    registry.emplace<Gate>(entity, open, closed, Gate::OPEN, Gate::CLOSED, false);
    registry.emplace<Render_component>(entity, closed);
    registry.emplace<Nonpassable>(entity);
    registry.emplace<Opaque>(entity);

    return entity;
}