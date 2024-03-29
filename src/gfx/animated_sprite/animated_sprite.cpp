#include "animated_sprite.h"

using namespace std::chrono_literals;


Animated_sprite::Animated_sprite(std::shared_ptr<Animated_sprite_frames> frames, unsigned int fps)
    : Animated_sprite(frames, fps != 0 ? 1000ms / fps : 0ms) {}

Animated_sprite::Animated_sprite(std::shared_ptr<Animated_sprite_frames> frames, std::chrono::milliseconds frame_time)
    : m_frames(frames), m_frame_time(frame_time)
{
    restart();
}

void Animated_sprite::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    if (m_frames && m_current_frame_idx < m_frames->frame_rects.size())
    {
        auto& vertices = m_frames->frame_rects[m_current_frame_idx].vertices;

        for (auto& i : vertices)
        {
            i.color = m_color;
        }

        st.transform *= getTransform();
        st.texture = m_frames->texture;

        rt.draw(vertices, 4, sf::Quads, st);
    }
}

void Animated_sprite::restart()
{
    restart(std::chrono::steady_clock::now());
}

void Animated_sprite::restart(std::chrono::steady_clock::time_point tp)
{
    m_frame_start_point = tp;
    m_current_frame_idx = 0;
}

void Animated_sprite::setFrameIdx(unsigned int idx, bool update_start_point)
{
    m_current_frame_idx = std::min(idx, frameCount()-1);
    if (update_start_point)
    {
        m_frame_start_point = std::chrono::steady_clock::now();
    }
}

void Animated_sprite::setFrameIdx(unsigned int idx, std::chrono::steady_clock::time_point tp)
{
    m_current_frame_idx = std::min(idx, frameCount() - 1);
    m_frame_start_point = tp;
}

unsigned int Animated_sprite::getFrameIdx() const
{
    return m_current_frame_idx;
}

unsigned int Animated_sprite::frameCount() const
{
    return m_frames ? m_frames->frame_rects.size() : 0;
}

void Animated_sprite::setRepeat(bool repeat)
{
    m_repeat = repeat;
}

bool Animated_sprite::isRepeating() const
{
    return m_repeat;
}

void Animated_sprite::updateFrameIdx()
{
    updateFrameIdx(std::chrono::steady_clock::now());
}

void Animated_sprite::updateFrameIdx(std::chrono::steady_clock::time_point tp)
{
    if (m_frame_time == 0ms) return;

    if (m_frames && m_frames->frame_rects.size() > 1)
    {
        std::chrono::steady_clock::time_point current_point = tp;

        unsigned int increase = (current_point - m_frame_start_point) / m_frame_time;

        if (increase > 0)
        {
            auto new_frame_idx = (m_current_frame_idx + increase) % m_frames->frame_rects.size();
            if (!isRepeating() && new_frame_idx < m_current_frame_idx) m_current_frame_idx = m_frames->frame_rects.size() - 1;
            else m_current_frame_idx = new_frame_idx;

            m_frame_start_point += m_frame_time * increase;
        }
    }
    else
    {
        m_current_frame_idx = 0;
    }
}

void Animated_sprite::setColor(sf::Color color)
{
    m_color = color;
}

sf::Color Animated_sprite::getColor() const
{
    return m_color;
}

sf::FloatRect Animated_sprite::getLocalBounds() const
{
    if (m_frames && m_current_frame_idx < m_frames->frame_rects.size())
    {
        auto& vertices     = m_frames->frame_rects[m_current_frame_idx].vertices;
        const float width  = vertices[3].position.x;
        const float height = vertices[1].position.y;

        return sf::FloatRect{ 0,0, width, height };
    }
    return sf::FloatRect{0,0,0,0};
}

sf::FloatRect Animated_sprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}