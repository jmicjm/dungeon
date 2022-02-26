#include "level_tile_map.h"
#include "../utils/visibleAreaBounds.h"
#include <algorithm>
#include <cmath>


Level_tile_map_chunk& Level_tile_map::at(const sf::Vector2i& pos)
{
    return chunks[pos.x][pos.y];
}

const Level_tile_map_chunk& Level_tile_map::at(const sf::Vector2i& pos) const
{
    return chunks[pos.x][pos.y];
}

void Level_tile_map::setSize(const sf::Vector2i& size)
{
    chunks.resize(size.x);
    for (auto& i : chunks)
    {
        i.resize(size.y);
    }
    setChunksTexture();
}

sf::Vector2i Level_tile_map::getSize() const
{
    return chunks.size() > 0 ? sf::Vector2i{ (int)chunks.size(), (int)chunks[0].size() } : sf::Vector2i{ 0, 0 };
}

void Level_tile_map::setChunksTexture()
{
    for (int x = 0; x < getSize().x; x++)
    {
        for (int y = 0; y < getSize().y; y++)
        {
            at({ x,y }).setTexture(&texture);
        }
    }
}

void Level_tile_map::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    if (getSize().x == 0 || getSize().y == 0)
    {
        return;
    }

    const auto [tl_px, br_px] = visibleAreaBoundsPixels(rt.getView());
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

Level_tile_map::Level_tile_map(const Level_tile_map& other) 
    : chunks(other.chunks), chunk_size_px(other.chunk_size_px), texture(other.texture)
{
    setChunksTexture();
}

Level_tile_map::Level_tile_map(Level_tile_map&& other)
    : chunks(std::move(other.chunks)), chunk_size_px(std::move(other.chunk_size_px)), texture(std::move(other.texture))
{
    setChunksTexture();
}

Level_tile_map& Level_tile_map::operator=(const Level_tile_map& other)
{
    chunks = other.chunks;
    chunk_size_px = other.chunk_size_px;
    texture = other.texture;
    setChunksTexture();

    return *this;
}

Level_tile_map& Level_tile_map::operator=(Level_tile_map&& other)
{
    chunks = std::move(other.chunks);
    chunk_size_px = std::move(other.chunk_size_px);
    texture = std::move(other.texture);
    setChunksTexture();

    return *this;
}

Level_tile_map::Level_tile_map(const Level_structure& ls, const sf::Vector2f& Tile_size, const sf::Vector2i& chunk_size)
{
    const int width  = std::ceil(ls.getSize().x * 1.f / chunk_size.x);
    const int height = std::ceil(ls.getSize().y * 1.f / chunk_size.y);

    setSize({ width, height});

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            const sf::IntRect area = { {x * chunk_size.x, y * chunk_size.y}, {chunk_size.x, chunk_size.y} };

            at({ x,y }) = Level_tile_map_chunk{ ls, Tile_size, area, &texture };
        }
    }
    chunk_size_px = { chunk_size.x*(int)Tile_size.x, chunk_size.y*(int)Tile_size.y };
}