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
    for (int i = 0; i < params.level_count; i++)
    {
        levels.push_back(std::make_shared<Level>(Level_params{ { 30,30 }, params.level_params }));
    }

    for (int i = 0; i < levels.size()-1; i++)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_down.png"), { 0,0,64,64 });
        levels[i]->entrances.insert({levels[i]->getStructure().getRoomRect(0).tl, Entrance(levels[i+1], levels[i+1]->getStructure().getRoomRect(0).tl, spr)});
    }
    for (int i = 1; i < levels.size(); i++)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_up.png"), { 0,0,64,64 });
        levels[i]->entrances.insert({ levels[i]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 }, Entrance(levels[i - 1], levels[i - 1]->getStructure().getRoomRect(0).tl + sf::Vector2i{ 1,0 }, spr) });
    }
    for (auto l : levels)
    {
        Primitive_sprite spr(Texture_bank::getTexture("assets/tiles/entrance_portal.png"), { 0,0,64,64 });
        l->entrances.insert({ l->getStructure().getRoomRect(0).tl + sf::Vector2i{ 0,1 }, Entrance(l, l->getStructure().getRoomRect(1).tl, spr) });
    }

    current_level = *levels.begin();
    current_level->entities.insert({ current_level->getStructure().getRoomRect(0).tl, player });
    player->world = this;
    static_cast<Entity*>(player.get())->level = current_level.get();
    player->setPosition(current_level->getStructure().getRoomRect(0).tl);
}

void World::update(const sf::RenderTarget& rt)
{
    current_level->update();
    current_level->updateVisibleTiles(player->getVisibleTiles(), rt);
}
