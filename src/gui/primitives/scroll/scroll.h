#pragma once
#include "../../gui_component.h"
#include "scroll_appearance.h"
#include "../button/button.h"
#include "../frame/frame.h"

namespace gui
{
    class Scroll_impl : public Gui_component
    {
        Button top_arrow;
        Button bottom_arrow;
        Frame handle;
        Frame line;

        int content_length = 0;
        int visible_content_length = -1;
        int top_position = 0;

        bool is_holding_handle = false;
        sf::Vector2i hold_source_pos;
        int hold_top_pos;

        void updateHandle();

        int visibleContentLength() const;

        void redraw() override;

        void resizeEvent(sf::Vector2f size_diff) override;
        void activateEvent() override;
        void deactivateEvent() override;

    protected:
        Scroll_impl();
        Scroll_impl(const Scroll_impl& other) = default;
        Scroll_impl(Scroll_impl&& other) = default;
        Scroll_impl& operator=(const Scroll_impl& other) = default;
        Scroll_impl& operator=(Scroll_impl&& other) = default;

        void linkChilds();

    public:
        void update() override;

        void setContentLength(int length);
        int  getContentLength() const;
        void setVisibleContentLength(int length);
        int  getVisibleContentLength() const;
        void setTopPosition(int position);
        int  getTopPosition() const;
        bool isNeeded() const;

        void up  (int n = 1);
        void down(int n = 1);

        void setAppearance(const Scroll_appearance& a);
        Scroll_appearance getAppearance() const;
    };

    class Scroll : public Scroll_impl
    {
    public:
        Scroll() = default;
        Scroll(const Scroll& other);
        Scroll(Scroll && other) noexcept;
        Scroll& operator=(const Scroll & other);
        Scroll& operator=(Scroll && other) noexcept;
    };
}