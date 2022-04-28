#pragma once
#include "../../gui_component.h"
#include "text_appearance.h"
#include "../scroll/scroll.h"
#include "../../../gfx/primitive_sprite.h"
#include "../../../gfx/copyable_rendertexture.h"

#include "SFML/Graphics/Font.hpp"

#include <string>


namespace gui
{
    class Text_impl : public Gui_component
    {
        std::string str;
        Copyable_rendertexture rendered_str;
        sf::Font font;
        unsigned int character_size = 24;
        float letter_spacing = 1;
        float line_spacing = 0;
        Scroll scroll;

        bool renderText(const std::u32string& str, bool with_scroll);
        void renderText();

        bool render_required = true;
        void redraw() override;

        void resizeEvent(sf::Vector2f size_diff) override;
        void activateEvent() override;
        void deactivateEvent() override;

    protected:
        Text_impl(sf::RenderWindow* rw);
        void linkChilds();

    public:
        void update() override;

        void setString(std::string str);
        const std::string& getString() const;

        void setFont(const std::string& filename);

        void setCharacterSize(unsigned int size);
        unsigned int getCharacterSize() const;

        void  setLetterSpacing(float spacing);
        float getLetterSpacing() const;

        void  setLineSpacing(float spacing);
        float getLineSpacing() const;

        void setAppearance(const Text_appearance& a);
        Text_appearance getAppearance() const;
    };

    class Text : public Text_impl
    {
    public:
        Text(sf::RenderWindow* rw);
        Text(const Text& other);
        Text(Text&& other) noexcept;
        Text& operator=(const Text& other);
        Text& operator=(Text&& other) noexcept;
    };
}