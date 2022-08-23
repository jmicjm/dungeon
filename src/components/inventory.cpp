#include "inventory.h"
#include "item.h"


bool defaultAllowCheck(const entt::registry& registry, entt::entity entity)
{
    return registry.try_get<Item>(entity);
}


Inventory::Inventory(unsigned int slot_count) 
    : Inventory(slot_count, defaultAllowCheck) {}

Inventory::Inventory(unsigned int slot_count, allow_f* allow_item_check)
    : slot_count(slot_count), allow_item_check(allow_item_check) {}

unsigned int Inventory::slotCount() const
{
    return slot_count;
}

bool Inventory::isSlotEmpty(unsigned int slot)
{
    return items.find(slot) == items.end();
}

auto Inventory::usedSlots() const -> const decltype(used_slots)&
{
    return used_slots;
}

bool Inventory::isAllowed(const entt::registry& registry, entt::entity entity) const
{
    return allow_item_check ? allow_item_check(registry, entity) : true;
}

bool Inventory::insert(const entt::registry& registry, entt::entity item, unsigned int slot)
{
    if (!isAllowed(registry, item) || slot >= slotCount() || !isSlotEmpty(slot)) return false;

    items.insert({ slot, item });
    used_slots.insert(slot);

    return true;
}

bool Inventory::insert(const entt::registry& registry, entt::entity item)
{
    if (!isAllowed(registry, item)) return false;

    const auto first_empty = [&] {
        if (used_slots.empty()) return 0u;
        return used_slots.begin()->upper();
    }();
    if (first_empty >= slotCount()) return false;

    items.insert({ first_empty, item });
    used_slots.insert(first_empty);

    return true;
}

entt::entity Inventory::remove(unsigned int slot)
{
    auto it = items.find(slot);
    if (it == items.end()) return entt::null;
    auto item = it->second;
    items.erase(it);
    used_slots.erase(slot);

    return item;
}

entt::entity Inventory::get(unsigned int slot)
{
    if (auto it = items.find(slot); it != items.end()) return it->second;
    return entt::null;
}
