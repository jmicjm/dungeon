#include "surface.h"

template<typename T>
static void resizeT(sf::Drawable& d, const sf::Vector2f& size)
{
    const sf::Vector2f spr_size = { static_cast<T&>(d).getGlobalBounds().width,
                                    static_cast<T&>(d).getGlobalBounds().height };
    static_cast<T&>(d).scale(size.x / spr_size.x, size.y / spr_size.y);
};

template<typename T>
static void setPositionT(sf::Drawable& d, const sf::Vector2f& position)
{
    static_cast<T&>(d).setPosition(position);
};

namespace gui
{
    void Surface::draw(sf::RenderTarget& rt, sf::RenderStates st) const
    {
        if (drawable)
        {
            if (predraw) predraw(*drawable);
            rt.draw(*drawable, st);
        }
    }

    Surface::Surface(const Surface& other)
        : drawable(other.drawable ? other.copy(*other.drawable) : nullptr),
          predraw(other.predraw),
          resize(other.resize),
          reposition(other.reposition),
          copy(other.copy) {}

    Surface::Surface(sf::RectangleShape rectangleShape)
        : Surface(std::move(rectangleShape),
                  [](sf::Drawable& d, const sf::Vector2f& size) { static_cast<sf::RectangleShape&>(d).setSize(size); },
                  setPositionT<sf::RectangleShape>) {}

    Surface::Surface(sf::Sprite sprite)
        : Surface(std::move(sprite),
                  resizeT<sf::Sprite>,
                  setPositionT<sf::Sprite>) {}

    Surface::Surface(Animated_sprite animated_sprite)
        : Surface(std::move(animated_sprite),
                  resizeT<Animated_sprite>,
                  setPositionT<Animated_sprite>,
                  [](sf::Drawable& d) { static_cast<Animated_sprite&>(d).updateFrameIdx(); }) {}

    Surface& Surface::operator=(const Surface& other)
    {
        drawable = other.drawable ? other.copy(*other.drawable) : nullptr;
        predraw = other.predraw;
        resize = other.resize;
        reposition = other.reposition;
        copy = other.copy;

        return *this;
    }

    void Surface::setSize(const sf::Vector2f& size)
    {
        if (drawable && resize) resize(*drawable, size);
    }

    void Surface::setPosition(const sf::Vector2f& position)
    {
        if (drawable && reposition) reposition(*drawable, position);
    }
}