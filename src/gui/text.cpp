#include "text.h"
#include <vector>

namespace gui
{
    void Text::prepareText()
    {
        std::vector<Primitive_sprite> prepared_text;

        const sf::Texture& tex = font.getTexture(character_size);

        const unsigned int width = getSize().x - scroll.getSize().x;
        unsigned int height = 0;

        unsigned int line = 0;
        unsigned int pos_in_line = 0;
        for (const auto& c : str)
        {
            sf::Glyph glyph = font.getGlyph(c, character_size, false);
            Primitive_sprite p_spr(&tex, glyph.textureRect);
            if (pos_in_line + glyph.advance >= width)
            {
                line++;
                pos_in_line = 0;
            }
            p_spr.move(sf::Vector2f( pos_in_line, line*character_size + character_size + glyph.bounds.top ));
            pos_in_line += glyph.advance;

            height = std::max(height, static_cast<unsigned int>(p_spr.vertices[1].position.y));

            prepared_text.push_back(p_spr);
        }

        rendered_str.create(width, height);
        rendered_str.clear({ 0,0,0,0 });
        for (const auto& c : prepared_text)
        {
            rendered_str.draw(c.vertices, 4, c.primitive_type, c.texture);
        }
        rendered_str.display();

        scroll.setContentLength(rendered_str.getSize().y);
    }

    void Text::redraw()
    {
        sf::Sprite str_sprite(rendered_str.getTexture());
        str_sprite.setTextureRect(
            { 
              sf::Vector2i(0, scroll.getTopPosition()),
              sf::Vector2i(rendered_str.getSize().x, std::min(static_cast<unsigned int>(getSize().y), rendered_str.getSize().y)) 
            }
        );

        draw(str_sprite);
        draw(scroll);
    }

    void Text::resizeEvent(const sf::Vector2i& size_diff)
    {
        prepareText();
    }

    Text::Text(sf::RenderWindow* rw) 
        : Gui_element(rw), scroll(rw)
    {
        scroll.setSizeInfo({ {16,0}, {0,1} });
        scroll.setPositionInfo({ {0,0}, {0,0}, {1,0} });
        scroll.setParent(this);
    }

    void Text::update()
    {
        scroll.update();
    }

    void Text::setString(std::string str)
    {
        this->str = str;
        prepareText();
    }

    const std::string& Text::getString() const
    {
        return str;
    }

    void Text::setFont(const std::string& filename)
    {
        font.loadFromFile(filename);
        prepareText();
    }

    void Text::setCharacterSize(unsigned int size)
    {
        character_size = size;
        prepareText();
    }

    unsigned int Text::getCharacterSize() const
    {
        return character_size;
    }
    void Text::setAppearance(const Text_appearance& a)
    {
        scroll.setAppearance(a.scroll);
    }
    Text_appearance Text::getAppearance() const
    {
        return { scroll.getAppearance() };
    }
}