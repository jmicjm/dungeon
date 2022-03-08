#include "world.h"
#include "../level/level.h"
#include "../level/level_params.h"
#include "../asset_storage/texture_bank.h"


void World::draw(sf::RenderTarget& rt, sf::RenderStates st) const
{
    rt.draw(*current_level, st);
}

World::World(const World_params& params, const std::shared_ptr<Player>& player)
    : player(player)
{
    for (const auto& lp : params.level_params)
    {
        levels.push_back(std::make_shared<Level>(lp));
    }

    for (int i = 0; i < levels.size()-1; i++)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_down.png"), { 0,0,64,64 });
        levels[i]->entrances.insert({ levels[i]->getStructure().getRoomRect(0).tl, Entrance{ { levels[i + 1], levels[i + 1]->getStructure().getRoomRect(0).tl }, spr } } );
    }
    for (int i = 1; i < levels.size(); i++)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_up.png"), { 0,0,64,64 });
        levels[i]->entrances.insert({ levels[i]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 }, Entrance{ { levels[i - 1], levels[i - 1]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 } }, spr } });
    }
    for (auto l : levels)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_portal.png"), { 0,0,64,64 });
        l->entrances.insert({ l->getStructure().getRoomRect(0).tl + sf::Vector2i{ 0,1 }, Entrance{ { l, l->getStructure().getRoomRect(1).tl }, spr } });
        l->entrances.insert({ l->getStructure().getRoomRect(1).tl + sf::Vector2i{ 0,1 }, Entrance{ { l, l->getStructure().getRoomRect(0).tl }, spr } });
    }

    current_level = *levels.begin();
    current_level->loadVisuals();
    current_level->entities.insert({ current_level->getStructure().getRoomRect(0).tl, player });
    player->world = this;
    player->level = current_level.get();
    player->setPosition(current_level->getStructure().getRoomRect(0).tl);
}

void World::update(const sf::RenderTarget& rt)
{
    current_level->update();
    current_level->updateVisibleTiles(player->getVisibleTiles(), rt);
}

std::shared_ptr<Level> World::changeLevel(std::shared_ptr<Level> new_level)
{
    if (current_level) current_level->unloadVisuals();
    if (new_level) new_level->loadVisuals();

    return std::exchange(current_level, new_level);
}
