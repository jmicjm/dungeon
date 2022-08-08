#include "text.h"
#include "../../../utils/utf8ToUtf32.h"
#include "../../../asset_storage/font_bank.h"

#include <vector>
#include <utility>
#include <cctype>


namespace gui
{
    void Text_impl::linkChilds()
    {
        scroll.setParent(this);
    }

    bool Text_impl::renderText(const std::u32string& str, bool with_scroll)
    {
        std::vector<Primitive_sprite> prepared_text;

        const sf::Texture& tex = font.getTexture(font_size);    

        const unsigned int width = size().x - with_scroll*scroll.size().x;
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

            sf::Glyph glyph = font.getGlyph(str[i], font_size, false);
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
                    word_width += advance(font.getGlyph(str[j], font_size, false));
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

            p_spr.move(sf::Vector2f( pos_in_line, line*(font_size+line_spacing) + font_size + glyph.bounds.top ));
            pos_in_line += advance(glyph);

            height = std::max(height, static_cast<unsigned int>(p_spr.vertices[1].position.y));

            if (!with_scroll && height >= size().y) return false;

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

    void Text_impl::renderText()
    {
        const std::u32string str = utf8ToUtf32(this->str);

        if (!renderText(str, false)) renderText(str, true);
    }

    void Text_impl::redraw()
    {
        if (std::exchange(render_required, false)) renderText();

        sf::Sprite str_sprite(rendered_str.getTexture());
        str_sprite.setTextureRect({ sf::Vector2i(0, scroll.getTopPosition()),
                                    sf::Vector2i(rendered_str.getSize().x, std::min(static_cast<unsigned int>(size().y), rendered_str.getSize().y)) });

        draw(str_sprite);
        if (rendered_str.getSize().y >= size().y) scroll.draw();
    }

    void Text_impl::resizeEvent(sf::Vector2f size_diff)
    {
        render_required = true;
    }

    void Text_impl::activateEvent()
    {
        scroll.activate();
    }

    void Text_impl::deactivateEvent()
    {
        scroll.deactivate();
    }

    Text_impl::Text_impl()
    {
        scroll.setSizeInfo({ {0,0}, {0,1} });
        scroll.setPositionInfo({ {0,0}, {0,0}, {1,0} });
        linkChilds();
        setAppearance(Text_appearance{});
    }

    void Text_impl::update()
    {
        scroll.update();
    }

    void Text_impl::setString(std::string str)
    {
        this->str = str;

        render_required = true;
    }

    const std::string& Text_impl::getString() const
    {
        return str;
    }

    void Text_impl::setFont(const std::string& filename)
    {
        font = *Font_bank::getFont(filename);
        font_name = filename;

        render_required = true;
    }

    void Text_impl::setFontSize(float size)
    {
        font_size = size;

        render_required = true;
    }

    float Text_impl::getFontSize() const
    {
        return font_size;
    }

    void Text_impl::setLetterSpacing(float spacing)
    {
        letter_spacing = spacing;

        render_required = true;
    }

    float Text_impl::getLetterSpacing() const
    {
        return letter_spacing;
    }

    void Text_impl::setLineSpacing(float spacing)
    {
        line_spacing = spacing;

        render_required = true;
    }

    float Text_impl::getLineSpacing() const
    {
        return line_spacing;
    }

    void Text_impl::setAppearance(const Text_appearance& a)
    {
        scroll.setAppearance(a.scroll);
        scroll.setSizeInfo({ {a.scroll_width,0}, {0,1} });
        setFont(a.font_name);
        font_size = a.font_size;
        letter_spacing = a.letter_spacing;
        line_spacing = a.line_spacing;

        render_required = true;
    }

    Text_appearance Text_impl::getAppearance() const
    {
        return { scroll.getAppearance(),
                 scroll.getSizeInfo().fixed.x,
                 font_name,
                 font_size,
                 letter_spacing,
                 line_spacing };
    }


    Text::Text(const Text& other)
        : Text_impl(other)
    {
        linkChilds();
    }

    Text::Text(Text&& other) noexcept
        : Text_impl(other)
    {
        linkChilds();
    }

    Text& Text::operator=(const Text& other)
    {
        linkChilds();
        return *this;
    }

    Text& Text::operator=(Text&& other) noexcept
    {
        linkChilds();
        return *this;
    }

}