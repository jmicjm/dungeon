#pragma once
#include "body_part_stats.h"
#include "body_part_attributes.h"
#include "../inventory.h"
#include <entt/entt.hpp>
#include <memory>
#include <vector>
#include <string>
#include <concepts>
#include <utility>
#include <optional>


enum class Body_part_type
{
    HEAD,
    NECK,
    UPPER_TORSO,
    LOWER_TORSO,
    FOREARM,
    ARM,
    HAND,
    THIGH,
    CALF,
    FOOT,
    JOINT
};

std::string toString(Body_part_type bpt);

struct Body_part;

class Body_node
{
    friend struct Body_part;

    Body_part* parent = nullptr;
    std::vector<std::unique_ptr<Body_part>> childs;

    Body_node() = default;
    Body_node(const Body_node& other);
    Body_node(Body_node&& other) noexcept;
    Body_node& operator=(const Body_node& other);
    Body_node& operator=(Body_node&& other) noexcept;

    void swap(Body_node& other);

public:
    Body_part* getParent();
    const Body_part* getParent() const;
    auto childCount() const -> decltype(childs)::size_type;
    Body_node& getChild(decltype(childs)::size_type idx);
    const Body_node& getChild(decltype(childs)::size_type idx) const;

    Body_part& addChild(Body_part part);
    Body_part& addChild(std::unique_ptr<Body_part> part);
    std::unique_ptr<Body_part> removeChild(decltype(childs)::size_type idx);

    void foreachChild(std::invocable<Body_part&> auto f);
    void foreachChild(std::invocable<const Body_part&> auto f) const;

    void foreachDescendant(std::invocable<Body_part&> auto f);
    void foreachDescendant(std::invocable<const Body_part&> auto f) const;
};

struct Body_part : Body_node
{
    Body_part_type type;
    std::string prefix;
    bool prefix_as_name = false;

    Body_part_stats stats;
    Body_part_base_attributes base_attributes;

    entt::entity inventory_entity = entt::null;

    Body_part(Body_part_type type, std::string prefix = "", bool prefix_as_name = false);
    Body_part(Body_part_type type, entt::registry& registry, Inventory inventory);
    Body_part(Body_part_type type, std::string prefix, bool prefix_as_name, entt::registry& registry, Inventory inventory);

    std::string name() const;
    std::string describe(const std::string& pronoun) const;

    Inventory* getInventory(entt::registry& registry);
    const Inventory* getInventory(const entt::registry& registry) const;

    std::optional<Body_part_stat_t> getStat(Body_part_stat stat) const;
    std::optional<Body_part_attribute_t> getAttribute(const entt::registry& registry, Body_part_attribute attr) const;
};


void Body_node::foreachChild(std::invocable<Body_part&> auto f)
{
    for (auto& child : childs) f(*child);
}

void Body_node::foreachChild(std::invocable<const Body_part&> auto f) const
{
    for (const auto& child : childs) f(const_cast<const Body_part&>(*child));
}

void Body_node::foreachDescendant(std::invocable<Body_part&> auto f)
{
    std::as_const(*this).foreachDescendant([&](const Body_part& bp) {
        f(const_cast<Body_part&>(bp)); 
    });
}

void Body_node::foreachDescendant(std::invocable<const Body_part&> auto f) const 
{
    foreachChild([&](const Body_part& bp) {
        f(bp);
        bp.foreachDescendant(f);
    });
}