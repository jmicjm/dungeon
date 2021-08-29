#pragma once
#include "gui_element.h"
#include "scroll.h"
#include "../gfx/primitive_sprite.h"

#include "SFML/Graphics/Font.hpp"

#include <string>
#include <vector>


namespace gui
{
    class Text : public Gui_element
    {
        std::string str;
        sf::RenderTexture rendered_str;
        sf::Font font;
        unsigned int character_size = 24;

        Scroll scroll;
        std::vector<Primitive_sprite> prepared_text;

        void prepareText();

        void redraw() override;

        void resizeEvent(const sf::Vector2i& size_diff) override;

    public:
        Text(sf::RenderWindow& rw);

        void update() override;

        void setString(std::string str);
        const std::string& getString() const;

        void setFont(const std::string& filename);

        void setCharacterSize(unsigned int size);
        unsigned int getCharacterSize() const;
    };
}