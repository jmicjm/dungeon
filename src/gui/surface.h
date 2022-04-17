#pragma once
#include "../gfx/animated_sprite/animated_sprite.h"

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"

#include <memory>


namespace gui
{
    class Surface : public sf::Drawable
    {
        std::unique_ptr<sf::Drawable> drawable = nullptr;

        using predraw_f = void(sf::Drawable&);
        using alter_dim_f = void(sf::Drawable&, const sf::Vector2f&);
        using copy_f = std::unique_ptr<sf::Drawable>(const sf::Drawable&);

        predraw_f* predraw = nullptr;
        alter_dim_f* resize = nullptr;
        alter_dim_f* reposition = nullptr;
        copy_f* copy = nullptr;

        void draw(sf::RenderTarget& rt, sf::RenderStates st) const override;

    public:
        Surface() {}
        Surface(const Surface& other);
        Surface(Surface&& other) noexcept = default;

        template<typename T>
        Surface(T drawable, alter_dim_f* resize_function, alter_dim_f* reposition_function, predraw_f* predraw_function = nullptr);

        /*convenience overloads for common types*/
        Surface(sf::RectangleShape rectangleShape);
        Surface(sf::Sprite sprite);
        Surface(Animated_sprite animated_sprite);

        Surface& operator=(const Surface& other);
        Surface& operator=(Surface&& other) noexcept = default;

        void setSize(const sf::Vector2f& size);
        void setPosition(const sf::Vector2f& position);
    };

    template<typename T>
    Surface::Surface(T drawable, alter_dim_f* resize_function, alter_dim_f* reposition_function, predraw_f* predraw_function)
        : drawable(std::make_unique<T>(std::move(drawable))),
          copy([](const sf::Drawable& drawable) -> std::unique_ptr<sf::Drawable> { return std::make_unique<T>(static_cast<const T&>(drawable)); }),
          resize(resize_function),
          reposition(reposition_function),
          predraw(predraw_function) {}
}