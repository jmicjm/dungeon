#include "createItem.h"
#include "../../components/item.h"
#include "../../components/weapon.h"
#include "../../components/render_component.h"
#include "../../asset_storage/item_rc_storage.h"
#include "../../gfx/zlevels.h"
#include <functional>
#include <unordered_map>


static void createSimpleWeapon(entt::registry& registry, const entt::entity item, const Weapon& weapon_component)
{
    registry.emplace<Weapon>(item, weapon_component);
}

namespace items
{
    entt::entity createItem(entt::registry& registry, const Item_id id)
    {
        using create_f = void(entt::registry& registry, entt::entity item);
        using namespace std::placeholders;

        static std::unordered_map<Item_id, std::function<create_f>> create_function_map =
        {
            { Item_id::LONGSWORD, std::bind(createSimpleWeapon, _1, _2, Weapon{ 32 }) },
            { Item_id::DAGGER, std::bind(createSimpleWeapon, _1, _2, Weapon{ 16 }) },
            { Item_id::BLOODTHIRSTY_BLADE, std::bind(createSimpleWeapon, _1, _2, Weapon{ 48 }) },
        };

        if (auto it = create_function_map.find(id); id != create_function_map.end())
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
