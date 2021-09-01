#pragma once
#include "gui_element.h"
#include "text_appearance.h"
#include "scroll.h"
#include "../gfx/primitive_sprite.h"

#include "SFML/Graphics/Font.hpp"

#include <string>


namespace gui
{
    class Text : public Gui_element
    {
        std::string str;
        sf::RenderTexture rendered_str;
        sf::Font font;
        unsigned int character_size = 24;
        Scroll scroll;

        void linkChilds();
        void prepareText();

        void redraw() override;
        void resizeEvent(const sf::Vector2i& size_diff) override;

    public:
        Text(sf::RenderWindow* rw);
        Text(const Text& other);
        Text(Text && other) noexcept;
        Text& operator=(const Text& other);
        Text& operator=(Text && other) noexcept;      

        void update() override;

        void setString(std::string str);
        const std::string& getString() const;

        void setFont(const std::string& filename);

        void setCharacterSize(unsigned int size);
        unsigned int getCharacterSize() const;

        void setAppearance(const Text_appearance& a);
        Text_appearance getAppearance() const;
    };
}