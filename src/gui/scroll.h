#pragma once
#include "gui_element.h"
#include "button.h"
#include "frame.h"

namespace gui
{
    class Scroll : public Gui_element
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

        bool redraw_required = true;
        void redraw(const sf::Vector2i& size_diff) override;
        void resizeEvent() override;

    public:
        Scroll(sf::RenderWindow& rw);

        void update() override;
        bool isRedrawRequired() override;

        void setContentLength(int length);
        int  getContentLength() const;
        void setVisibleContentLength(int length);
        int  getVisibleContentLength() const;
        void setTopPosition(int position);
        int  getTopPosition() const;

        void up  (int n = 1);
        void down(int n = 1);

        void setTopArrowAppearance(Surface pressed, Surface released, Surface hovered);
        void setBottomArrowAppearance(Surface pressed, Surface released, Surface hovered);
        void setHandleSurface(Surface s);
        void setLineSurface(Surface s);
    };
}