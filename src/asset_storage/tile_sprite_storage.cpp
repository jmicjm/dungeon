#include "tile_sprite_storage.h"
#include "texture_bank.h"

#include "SFML/Graphics/Rect.hpp"

#include <string>


Tile_sprite_storage::Tile_sprite_map Tile_sprite_storage::sprite_map = Tile_sprite_map();

void Tile_sprite_storage::loadSprites()
{
    using namespace TILE_SPRITE_ID;
    struct s_data
    {
        struct spr 
        { 
            std::string path;
            sf::IntRect rect;
            sf::Vector2f offset = { 0,0 }; 
            float chance = 1;
            spr(std::string path, sf::IntRect rect, sf::Vector2f offset, float chance = 1) : path(path), rect(rect), offset(offset), chance(chance) {}
            spr(std::string path, sf::IntRect rect, float chance) : path(path), rect(rect), chance(chance) {}
            spr(std::string path, sf::IntRect rect) : path(path), rect(rect) {}
        };
        tile_sprite_id_t tile_type_id;
        std::vector<spr> variants;
    };
    static std::vector<s_data> data =
    {
        #include "tile_sprite_list.h"
    };

    for (auto& i : data)
    {
        std::vector<Sprite_chance> tmp_spr_vec;

        for (auto& j : i.variants)
        {
            const sf::Texture* tex_ptr = Texture_bank::getTexture(j.path);
            Sprite_chance sc{ {tex_ptr, j.rect} };;
            sc.sprite.move(j.offset);
            sc.chance = j.chance;
            tmp_spr_vec.push_back(sc);
        }
        sprite_map.insert({ i.tile_type_id, tmp_spr_vec });
    }
}

std::vector<Tile_sprite_storage::Sprite_chance>* Tile_sprite_storage::getSprite(TILE_SPRITE_ID::tile_sprite_id_t key)
{
    auto it = sprite_map.find(key);
    if (it != sprite_map.end())
    {
        return &it->second;
    }
    else
    {
        return nullptr;
    }
}
