#include "createItem.h"
#include "../../components/item.h"
#include "../../components/weapon.h"
#include "../../components/render_component.h"
#include "../../components/stackable_item.h"
#include "../../components/coins.h"
#include "../../components/description.h"
#include "../../asset_storage/item_rc_storage.h"
#include "../../gfx/zlevels.h"
#include <functional>
#include <unordered_map>
#include <string>


static void createSimpleWeapon(entt::registry& registry, const entt::entity item, const Weapon& weapon_component, std::string name)
{
    registry.emplace<Weapon>(item, weapon_component);
    registry.emplace<Description>(item, name);
}

static void createCoins(entt::registry& registry, const entt::entity item)
{
    registry.emplace<Coins>(item);
    registry.emplace<Stackable_item>(item, item, 999);
    registry.emplace<Description>(item, "coins");
}

namespace items
{
    entt::entity createItem(entt::registry& registry, const Item_id id)
    {
        using create_f = void(entt::registry& registry, entt::entity item);
        using namespace std::placeholders;

        static std::unordered_map<Item_id, std::function<create_f>> create_function_map =
        {
            { Item_id::LONGSWORD, std::bind(createSimpleWeapon, _1, _2, Weapon{ 32 }, "longsword")},
            { Item_id::DAGGER, std::bind(createSimpleWeapon, _1, _2, Weapon{ 16 }, "dagger")},
            { Item_id::BLOODTHIRSTY_BLADE, std::bind(createSimpleWeapon, _1, _2, Weapon{ 48 }, "bloodthirsty blade")},
            { Item_id::COINS, createCoins },
        };

        if (auto it = create_function_map.find(id); it != create_function_map.end())
        {
            auto item = registry.create();
            registry.emplace<Item>(item, id);
            registry.emplace<Render_component>(item, Item_rc_storage::getRc(id));
            it->second(registry, item);

            return item;
        }
        return entt::null;
    }
}
