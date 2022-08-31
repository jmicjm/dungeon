#include "stackable_item.h"
#include "item.h"
#include "../entities/cloneEntity.h"
#include <algorithm>


bool Stackable_item::defaultCanStack(const entt::registry& registry, entt::entity self, entt::entity other)
{
    const auto item_a = registry.try_get<Item>(self);
    const auto item_b = registry.try_get<Item>(other);
    if (item_a && item_b)
    {
        return item_a->id == item_b->id;
    }
    return false;
}

void Stackable_item::defaultStackWith(entt::registry& registry, entt::entity self, entt::entity other, int amount)
{
    return;
}

Stackable_item::Stackable_item(entt::entity self, int max_amount, stack_with_f* stack_with, can_stack_f* can_stack_check)
    : self(self), max_amount(std::max(1, max_amount)), amount(1), can_stack_check(can_stack_check) {}

bool Stackable_item::canStackWith(entt::registry& registry, entt::entity other)
{
    if (registry.try_get<Stackable_item>(other) && can_stack_check) return can_stack_check(registry, self, other);
    return false;
}

void Stackable_item::stackWith(entt::registry& registry, entt::entity other)
{
    if (!canStackWith(registry, other)) return;

    if (auto other_stackable = registry.try_get<Stackable_item>(other))
    {
        const auto amount_to_take = std::min(other_stackable->amount, max_amount - amount);
        if (stack_with) stack_with(registry, self, other, amount_to_take);
        amount += amount_to_take;
        other_stackable->amount -= amount_to_take;
    }
}

entt::entity Stackable_item::take(entt::registry& registry, int amount)
{
    auto new_item = cloneEntity(registry, self);

    registry.get<Stackable_item>(new_item).self = new_item;
    registry.get<Stackable_item>(new_item).amount = std::min(this->amount, amount);
    this->amount = std::max(0, this->amount - amount);

    return new_item;
}

void Stackable_item::setAmount(int amount)
{
    this->amount = std::min(amount, max_amount);
}

int Stackable_item::getAmount() const
{
    return amount;
}

int Stackable_item::getMaxAmount() const
{
    return max_amount;
}
