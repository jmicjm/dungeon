#pragma once
#include <unordered_map>
#include <concepts>
#include <boost/icl/interval_set.hpp>
#include <entt/entt.hpp>

class Inventory
{
    std::unordered_map<unsigned int, entt::entity> items;
    boost::icl::interval_set<unsigned int, std::less, boost::icl::right_open_interval<unsigned int>> used_slots;
    unsigned int slot_count;

    using allow_f = bool(const entt::registry&, entt::entity, const Inventory& inventory);
    allow_f* allow_item_check;

    entt::entity associated_body = entt::null;

    void updateBody(entt::registry& registry);

public:
    Inventory(unsigned int slot_count);
    Inventory(unsigned int slot_count, allow_f* allow_item_check);
    unsigned int slotCount() const;
    bool isSlotEmpty(unsigned int slot);
    auto usedSlots() const -> const decltype(used_slots)&;
    bool isAllowed(const entt::registry& registry, entt::entity entity) const;
    bool insert(entt::registry& registry, entt::entity item, unsigned int slot);
    bool insert(entt::registry& registry, entt::entity item);
    entt::entity remove(entt::registry& registry, unsigned int slot);
    entt::entity get(unsigned int slot) const;
    void setAssociatedBody(entt::entity body);
    entt::entity getAssociatedBody() const;
    void foreach(std::invocable<entt::entity> auto f) const;
};

void Inventory::foreach(std::invocable<entt::entity> auto f) const
{
    for (auto item : items) f(item.second);
}

template<typename T>
auto countTInInventory(const entt::registry& registry, const Inventory& inventory)
{
    auto c = 0;
    inventory.foreach([&](auto entity) {
        if (registry.try_get<T>(entity)) c++;
    });
    return c;
}