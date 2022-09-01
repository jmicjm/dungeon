#pragma once
#include <entt/entt.hpp>

class Stackable_item
{
    entt::entity self;

    int max_amount;
    int amount;

    using can_stack_f = bool(const entt::registry& registry, entt::entity self, entt::entity other);
    can_stack_f* can_stack_check = nullptr;
    using stack_with_f = void(entt::registry& registry, entt::entity self, entt::entity other, int amount);
    stack_with_f* stack_with = nullptr;

    static bool defaultCanStack(const entt::registry& registry, entt::entity self, entt::entity other);
    static void defaultStackWith(entt::registry& registry, entt::entity self, entt::entity other, int amount);

public:
    Stackable_item(entt::entity self, int max_amount, stack_with_f* stack_with = defaultStackWith, can_stack_f* can_stack_check = defaultCanStack);

    bool canStackWith(entt::registry& registry, entt::entity other);
    void stackWith(entt::registry& registry, entt::entity other);
    entt::entity take(entt::registry& registry, int amount);
    void setAmount(int amount);
    int getAmount() const;
    int getMaxAmount() const;
    bool isEmpty() const;
};