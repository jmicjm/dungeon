#pragma once
#include "body_part.h"
#include <concepts>
#include <utility>


struct Body_graph
{
    std::unique_ptr<Body_part> root;

    Body_graph() = default;
    Body_graph(const Body_graph& other) 
    {
        root = other.root ? std::make_unique<Body_part>(*other.root) : nullptr;
    }
    Body_graph(Body_graph&& other) noexcept = default;
    Body_graph& operator=(const Body_graph& other)
    {
        root = other.root ? std::make_unique<Body_part>(*other.root) : nullptr;
        return *this;
    }
    Body_graph& operator=(Body_graph&& other) noexcept = default;

    void foreach(std::invocable<Body_part&> auto f);
    void foreach(std::invocable<const Body_part&> auto f) const;
};

void Body_graph::foreach(std::invocable<Body_part&> auto f)
{
    if (root)
    {
        f(*root);
        root->foreachDescendant(f);
    }
}

void Body_graph::foreach(std::invocable<const Body_part&> auto f) const
{
    if (root)
    {
        f(std::as_const(*root));
        std::as_const(root)->foreachDescendant(f);
    }
}