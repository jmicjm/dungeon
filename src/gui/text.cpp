#include "text.h"
#include "../utils/utf8ToUtf32.h"

#include <vector>
#include <utility>
#include <cctype>


namespace gui
{
    void Text::linkChilds()
    {
        scroll.setParent(this);
    }

    bool Text::renderText(const std::u32string& str, bool with_scroll)
    {
        std::vector<Primitive_sprite> prepared_text;

        const sf::Texture& tex = font.getTexture(character_size);    

        const unsigned int width = getSize().x - with_scroll*scroll.getSize().x;
        unsigned int height = 0;

        unsigned int line = 0;
        float pos_in_line = 0;
        for (int i = 0; i < str.size(); i++)
        {
            auto moveToNextLine = [&]()
            {
                line++;
                pos_in_line = 0;
            };

            if (str[i] == U'\n')
            {
                moveToNextLine();
                continue;
            }            

            sf::Glyph glyph = font.getGlyph(str[i], character_size, false);
            Primitive_sprite p_spr(&tex, glyph.textureRect);

            auto advance = [&](const sf::Glyph& glyph)
            {
                return glyph.bounds.width ? glyph.bounds.width + letter_spacing : glyph.advance;;
            };
            auto isWB = [](const char32_t& c)
            {
                return std::iscntrl(c) || std::isblank(c) || std::ispunct(c);
            };
            auto fitsInLine = [&]()
            {
                float word_width = 0;
                for (int j = i; 
                     j < str.size() && !isWB(str[j]) && pos_in_line + word_width <= width;
                     j++)
                {
                    word_width += advance(font.getGlyph(str[j], character_size, false));
                }
                return pos_in_line + word_width <= width;
            };

            if (pos_in_line != 0 && ((i>0 ? isWB(str[i-1]) : true) && !fitsInLine()) || pos_in_line + advance(glyph) > width)
            {
                moveToNextLine();
            }
            if (pos_in_line == 0 && std::isblank(str[i]))//skip spaces at the beginning of line
            {
                continue;
            }

            p_spr.move(sf::Vector2f( pos_in_line, line*(character_size+line_spacing) + character_size + glyph.bounds.top ));
            pos_in_line += advance(glyph);

            height = std::max(height, static_cast<unsigned int>(p_spr.vertices[1].position.y));

            if (!with_scroll && height >= getSize().y) return false;

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

        return true;
    }

    void Text::renderText()
    {
        const std::u32string str = utf8ToUtf32(this->str);

        if (!renderText(str, false)) renderText(str, true);
    }

    void Text::redraw()
    {
        if (std::exchange(redraw_required, false))
        {
            renderText();
        }

        sf::Sprite str_sprite(rendered_str.getTexture());
        str_sprite.setTextureRect(
            { 
              sf::Vector2i(0, scroll.getTopPosition()),
              sf::Vector2i(rendered_str.getSize().x, std::min(static_cast<unsigned int>(getSize().y), rendered_str.getSize().y)) 
            }
        );

        draw(str_sprite);
        if(rendered_str.getSize().y >= getSize().y) draw(scroll, false);  
    }

    void Text::resizeEvent(const sf::Vector2i& size_diff)
    {
        redraw_required = true;
    }

    Text::Text(sf::RenderWindow* rw)
        : Gui_element(rw), scroll(rw)
    {
        scroll.setSizeInfo({ {0,0}, {0,1} });
        scroll.setPositionInfo({ {0,0}, {0,0}, {1,0} });
        linkChilds();
    }

    Text::Text(const Text& other)
        : Gui_element(other),
        str(other.str),
        font(other.font),
        character_size(other.character_size),
        scroll(other.scroll)
    {
        linkChilds();
    }

    Text::Text(Text&& other) noexcept
        : Gui_element(std::move(other)),
        str(std::move(other.str)),
        font(std::move(other.font)),
        character_size(std::move(other.character_size)),
        scroll(std::move(other.scroll))
    {
        linkChilds();
    }

    Text& Text::operator=(const Text& other)
    {
        Gui_element::operator=(other);
        str = other.str;
        font = other.font;
        character_size = other.character_size;
        scroll = other.scroll;

        rendered_str.~RenderTexture();
        new(&rendered_str) sf::RenderTexture;

        linkChilds();

        return *this;
    }

    Text& Text::operator=(Text&& other) noexcept
    {
        Gui_element::operator=(std::move(other));
        str = std::move(other.str);
        font = std::move(other.font);
        character_size = std::move(other.character_size);
        scroll = std::move(other.scroll);

        rendered_str.~RenderTexture();
        new(&rendered_str) sf::RenderTexture;

        linkChilds();

        return *this;
    }

    void Text::update()
    {
        scroll.update();
    }

    bool Text::isRedrawRequired() const
    {
        return redraw_required || scroll.isRedrawRequired();
    }

    void Text::setString(std::string str)
    {
        this->str = str;

        redraw_required = true;
    }

    const std::string& Text::getString() const
    {
        return str;
    }

    void Text::setFont(const std::string& filename)
    {
        font.loadFromFile(filename);

        redraw_required = true;
    }

    void Text::setCharacterSize(unsigned int size)
    {
        character_size = size;

        redraw_required = true;
    }

    unsigned int Text::getCharacterSize() const
    {
        return character_size;
    }

    void Text::setLetterSpacing(float spacing)
    {
        letter_spacing = spacing;

        redraw_required = true;
    }

    float Text::getLetterSpacing() const
    {
        return letter_spacing;
    }

    void Text::setLineSpacing(float spacing)
    {
        line_spacing = spacing;

        redraw_required = true;
    }

    float Text::getLineSpacing() const
    {
        return line_spacing;
    }

    void Text::setAppearance(const Text_appearance& a)
    {
        scroll.setAppearance(a.scroll);
        scroll.setSizeInfo({ {a.scroll_width,0}, {0,1} });

        redraw_required = true;
    }

    Text_appearance Text::getAppearance() const
    {
        return { scroll.getAppearance(), scroll.getSizeInfo().fixed.x };
    }
}