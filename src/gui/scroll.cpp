#include "scroll.h"
#include "../input/input.h"


namespace gui
{
    void Scroll_impl::updateHandle()
    {
        const int area_len = getSize().y - top_arrow.getSize().y - bottom_arrow.getSize().y;

        const float visible_perc = getContentLength() > 0 ? static_cast<float>(visibleContentLength()) / getContentLength() : 1;
        const int handle_len = std::clamp(static_cast<int>(area_len * visible_perc), 1, std::max(1, area_len));
        handle.setSizeInfo({ sf::Vector2f(0, handle_len), { 1,0 } });

        const float pos_perc = static_cast<float>(getTopPosition()) / (getContentLength() - visibleContentLength());
        const int handle_pos = top_arrow.getSize().y + (area_len - handle_len) * pos_perc;
        handle.setPositionInfo({ { 0,handle_pos } });
    }

    int Scroll_impl::visibleContentLength() const
    {
        return visible_content_length < 0 ? getSize().y : visible_content_length;
    }

    void Scroll_impl::redraw()
    {
        draw(line);
        draw(top_arrow);
        draw(bottom_arrow);
        updateHandle();
        draw(handle);

        redraw_required = false;
    }

    bool Scroll_impl::isRedrawRequired() const
    {
        return redraw_required 
            || top_arrow.isRedrawRequired() 
            || bottom_arrow.isRedrawRequired() 
            || handle.isRedrawRequired() 
            || line.isRedrawRequired();
    }

    void Scroll_impl::resizeEvent(const sf::Vector2i& size_diff)
    {
        setTopPosition(getTopPosition());
    }

    void Scroll_impl::activateEvent()
    {
        top_arrow.activate();
        bottom_arrow.activate();
    }

    void Scroll_impl::deactivateEvent()
    {
        top_arrow.deactivate();
        bottom_arrow.deactivate();
        is_holding_handle = false;
    }

    Scroll_impl::Scroll_impl(sf::RenderWindow* rw)
        : Gui_component(rw), top_arrow(rw), bottom_arrow(rw), handle(rw), line(rw)
    {        
        top_arrow.setPressDelay(std::chrono::milliseconds(0));
        bottom_arrow.setPressDelay(std::chrono::milliseconds(0));
        
        linkChilds();

        top_arrow.setSizeInfo({ {0,0}, {1,0} });
        bottom_arrow.setSizeInfo({ {0,0}, {1,0} });
        bottom_arrow.setPositionInfo({ {0,0}, {0,0}, {0,1} });
        line.setSizeInfo({ { 0,0 }, { 1,1 } });
    }

    void Scroll_impl::linkChilds()
    {
        top_arrow.setParent(this);
        top_arrow.setPressFunction([&]() {up(10); });
        bottom_arrow.setParent(this);
        bottom_arrow.setPressFunction([&]() {down(10); });
        handle.setParent(this);
        line.setParent(this);
    }

    void Scroll_impl::update()
    {
        if (!isActive()) return;

        if (!is_holding_handle)
        {
            top_arrow.update();
            bottom_arrow.update();
        }

        auto hold_vec = Input::getMouseHoldVec(sf::Mouse::Left);
        if (hold_vec)
        {
            if (is_holding_handle && hold_vec->source_pos != hold_source_pos)
            {
                is_holding_handle = false;
            }

            if (!is_holding_handle && handle.visibleArea().contains(sf::Vector2f{ hold_vec->source_pos }))
            {
                is_holding_handle = true;
                hold_source_pos = hold_vec->source_pos;
                hold_top_pos = getTopPosition();
            }
                 
            if (is_holding_handle)
            {      
                const int area_len = getSize().y - top_arrow.getSize().y - bottom_arrow.getSize().y - handle.getSize().y;
                const int shift = hold_vec->current_pos.y - hold_vec->source_pos.y;
                const float perc_shift = static_cast<float>(shift) / area_len;

                setTopPosition(hold_top_pos + (getContentLength() - visibleContentLength()) * perc_shift);
            }
        }
        else
        {
            is_holding_handle = false;
        }

    }

    void Scroll_impl::setContentLength(int length)
    {
        content_length = length;
        setTopPosition(getTopPosition());
    }

    int Scroll_impl::getContentLength() const
    {
        return content_length;
    }

    void Scroll_impl::setVisibleContentLength(int length)
    {
        visible_content_length = length;
        redraw_required = true;
    }

    int Scroll_impl::getVisibleContentLength() const
    {
        return visible_content_length;
    }

    void Scroll_impl::setTopPosition(int position)
    {
        top_position = std::clamp(position, 0, std::max(0,content_length - visibleContentLength()));
        redraw_required = true;
    }

    int Scroll_impl::getTopPosition() const
    {
        return top_position;
    }

    void Scroll_impl::up(int n)
    {
        setTopPosition(top_position - n);
    }

    void Scroll_impl::down(int n)
    {
        setTopPosition(top_position + n);
    }

    void Scroll_impl::setAppearance(const Scroll_appearance& a)
    {
        line.setAppearance(a.line);
        handle.setAppearance(a.handle);
        top_arrow.setAppearance(a.top_button);
        bottom_arrow.setAppearance(a.bottom_button);

        const auto& f = a.button_fixed_height;
        const auto& p = a.button_percentage_height;

        top_arrow.setSizeInfo({ {0,f}, {1,p} });
        bottom_arrow.setSizeInfo({ {0,f}, {1,p} });

        redraw_required = true;
    }

    Scroll_appearance Scroll_impl::getAppearance() const
    {
        Scroll_appearance a;
        a.line = line.getAppearance();
        a.handle = handle.getAppearance();
        a.top_button = top_arrow.getAppearance();
        a.bottom_button = bottom_arrow.getAppearance();
        a.button_fixed_height = top_arrow.getSizeInfo().fixed.y;
        a.button_percentage_height = top_arrow.getSizeInfo().percentage.y;

        return a;
    }

    Scroll::Scroll(sf::RenderWindow* rw) 
        : Scroll_impl(rw) {}

    Scroll::Scroll(const Scroll& other) 
        : Scroll_impl(other)
    {
        linkChilds();
    }

    Scroll::Scroll(Scroll&& other) noexcept
        : Scroll_impl(std::move(other))
    {
        linkChilds();
    }

    Scroll& Scroll::operator=(const Scroll& other)
    {
        Scroll_impl::operator=(other);
        linkChilds();

        return *this;
    }

    Scroll& Scroll::operator=(Scroll&& other) noexcept
    {
        Scroll_impl::operator=(std::move(other));
        linkChilds();

        return *this;

    }
}