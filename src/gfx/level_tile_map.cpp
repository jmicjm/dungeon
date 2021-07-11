#include "level_tile_map.h"
#include <algorithm>
#include <cmath>


level_tile_map_chunk& level_tile_map::at(const sf::Vector2i& pos)
{
	return chunks[pos.x][pos.y];
}

const level_tile_map_chunk& level_tile_map::at(const sf::Vector2i& pos) const
{
    return chunks[pos.x][pos.y];
}

void level_tile_map::setSize(const sf::Vector2i& size)
{
    chunks.resize(size.x);
    for (auto& i : chunks)
    {
        i.resize(size.y);
    }
}

sf::Vector2i level_tile_map::getSize() const
{
    return chunks.size() > 0 ? sf::Vector2i{ (int)chunks.size(), (int)chunks[0].size() } : sf::Vector2i{ 0, 0 };
}

void level_tile_map::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    const sf::View view = rt.getView();

    const sf::FloatRect view_rect = { view.getCenter() - view.getSize()/2.f, view.getSize() };
    const sf::FloatRect bounding_box = sf::Transform{}.rotate(view.getRotation(), view.getCenter()).transformRect(view_rect);

    const sf::Vector2f tl_px = { bounding_box.left, bounding_box.top };
    const sf::Vector2f br_px = tl_px + sf::Vector2f{bounding_box.width, bounding_box.height};

    const sf::Vector2i tl_chunk = 
    {
        std::max(0, (int)tl_px.x / chunk_size_px.x),
        std::max(0, (int)tl_px.y / chunk_size_px.y) 
    };
    const sf::Vector2i br_chunk = 
    {
        std::min(getSize().x-1, (int)br_px.x / chunk_size_px.x),
        std::min(getSize().y-1, (int)br_px.y / chunk_size_px.y)
    };

    for (int x = tl_chunk.x; x <= br_chunk.x; x++)
    {
        for (int y = tl_chunk.y; y <= br_chunk.y; y++)
        {
            rt.draw(at({ x,y }), st);
        }
    }
}

void level_tile_map::populate(const level_structure& ls, const sf::Vector2f& tile_size, const sf::Vector2i& chunk_size)
{
    const int width  = std::ceil(ls.getSize().x * 1.f / chunk_size.x);
    const int height = std::ceil(ls.getSize().y * 1.f / chunk_size.y);

    setSize({ width, height});

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            const sf::IntRect area = { {x * chunk_size.x, y * chunk_size.y}, {chunk_size.x, chunk_size.y} };

            at({ x,y }).populate(ls, tile_size, area);
        }
    }
    chunk_size_px = { chunk_size.x*(int)tile_size.x, chunk_size.y*(int)tile_size.y };
}