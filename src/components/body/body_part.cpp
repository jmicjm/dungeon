#include "body_part.h"


Body_part_base::Body_part_base(const Body_part_base& other)
    : parent{ other.parent }
{
    childs.resize(other.childs.size());
    for (auto i = 0; i < other.childs.size(); i++)
    {
        if (other.childs[i])
        {
            childs[i] = std::make_unique<Body_part>(*other.childs[i]);
            childs[i]->parent = static_cast<Body_part*>(this);
        }
    }
}

Body_part_base::Body_part_base(Body_part_base&& other) noexcept
  : parent{ other.parent },
    childs{ std::move(other.childs) } 
{
    for (const auto& child : childs)
    {
        if (child)
        {
            child->parent = static_cast<Body_part*>(this);
        }
    }
}

Body_part_base& Body_part_base::operator=(const Body_part_base& other)
{
    Body_part_base{ other }.swap(*this);
    return *this;
}

Body_part_base& Body_part_base::operator=(Body_part_base&& other) noexcept
{
    Body_part_base{ std::move(other) }.swap(*this);
    return *this;
}

void Body_part_base::swap(Body_part_base& other)
{
    std::swap(parent, other.parent);
    std::swap(childs, other.childs);
}



Body_part::Body_part(Body_part_type type, std::string prefix)
  : type{ type },
    prefix{ std::move(prefix) } {}

Body_part& Body_part::addChild(Body_part part)
{
    childs.push_back(std::make_unique<Body_part>(part));
    childs.back()->parent = static_cast<Body_part*>(this);

    return *this;
}

std::string Body_part::name() const
{
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

std::string toString(Body_part_type bpt)
{
    using enum Body_part_type;
    switch (bpt)
    {
    case HEAD: return "head";
    case TORSO: return "torso";
    case ARM: return "arm";
    case HAND: return "hand";
    case LEG: return "leg";
    case FOOT: return "foot";
    default: return "";
    }
}
