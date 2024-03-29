#include "body_part.h"


Body_part* Body_node::getParent()
{
    return parent;
}

const Body_part* Body_node::getParent() const
{
    return parent;
}

Body_part& Body_node::addChild(Body_part part)
{
    childs.push_back(std::make_unique<Body_part>(part));
    childs.back()->parent = static_cast<Body_part*>(this);

    return static_cast<Body_part&>(*this);
}

Body_part& Body_node::addChild(std::unique_ptr<Body_part> part)
{
    if (part)
    {
        childs.push_back(std::move(part));
        childs.back()->parent = static_cast<Body_part*>(this);
    }

    return static_cast<Body_part&>(*this);
}

std::unique_ptr<Body_part> Body_node::removeChild(decltype(childs)::size_type idx)
{
    auto bp = std::move(childs[idx]);
    bp->parent = nullptr;
    childs.erase(childs.begin() + idx);

    return bp;
}

auto Body_node::childCount() const -> decltype(childs)::size_type
{
    return childs.size();
}

Body_node& Body_node::getChild(decltype(childs)::size_type idx)
{
    return *childs[idx];
}

const Body_node& Body_node::getChild(decltype(childs)::size_type idx) const
{
    return *childs[idx];
}

Body_node::Body_node(const Body_node& other)
    : parent{ other.parent }
{
    childs.resize(other.childs.size());
    for (auto i = 0; i < other.childs.size(); i++)
    {
        childs[i] = std::make_unique<Body_part>(*other.childs[i]);
        childs[i]->parent = static_cast<Body_part*>(this);
    }
}

Body_node::Body_node(Body_node&& other) noexcept
  : parent{ other.parent },
    childs{ std::move(other.childs) } 
{
    for (const auto& child : childs)
    {
        child->parent = static_cast<Body_part*>(this);
    }
}

Body_node& Body_node::operator=(const Body_node& other)
{
    Body_node{ other }.swap(*this);
    return *this;
}

Body_node& Body_node::operator=(Body_node&& other) noexcept
{
    Body_node{ std::move(other) }.swap(*this);
    return *this;
}

void Body_node::swap(Body_node& other)
{
    std::swap(parent, other.parent);
    std::swap(childs, other.childs);
}



Body_part::Body_part(Body_part_type type, std::string prefix, bool prefix_as_name)
  : type{ type },
    prefix{ std::move(prefix) },
    prefix_as_name{ prefix_as_name } {}

Body_part::Body_part(Body_part_type type, entt::registry& registry, Inventory inventory)
    : Body_part{ type, "", false, registry, inventory} {}

Body_part::Body_part(Body_part_type type, std::string prefix, bool prefix_as_name, entt::registry& registry, Inventory inventory)
    : Body_part{ type, prefix, prefix_as_name }
{
    inventory_entity = registry.create();
    registry.emplace<Inventory>(inventory_entity, std::move(inventory));
}

std::string Body_part::name() const
{
    if (prefix_as_name) return prefix;
    return (prefix.empty() ? "" : prefix + " ") + toString(type);
}

std::string Body_part::describe(const std::string& pronoun) const
{
    std::string desc = pronoun + " has " + name();
    if (parent) desc += " connected to " + parent->name();
    desc += ".";

    for (const auto& child : childs)
    {
        if (child) desc += " " + child->describe(pronoun);
    }

    return desc;
}

Inventory* Body_part::getInventory(entt::registry& registry)
{
    return registry.valid(inventory_entity) ? registry.try_get<Inventory>(inventory_entity) : nullptr;
}

std::string toString(Body_part_type bpt)
{
    using enum Body_part_type;
    switch (bpt)
    {
    case HEAD: return "head";
    case NECK: return "neck";
    case UPPER_TORSO: return "upper torso";
    case LOWER_TORSO: return "lower torso";
    case FOREARM: return "forearm";
    case ARM: return "arm";
    case HAND: return "hand";
    case THIGH: return "thigh";
    case CALF: return "calf";
    case FOOT: return "foot";
    case JOINT: return "joint";
    default: return "";
    }
}
