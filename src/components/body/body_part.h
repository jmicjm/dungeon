#pragma once
#include <memory>
#include <vector>
#include <string>


enum class Body_part_type
{
    HEAD,
    TORSO,
    ARM,
    HAND,
    LEG,
    FOOT
};

std::string bodyPartTypeToString(Body_part_type bpt);


struct Body_part;

struct Body_part_base
{
    Body_part* parent = nullptr;
    std::vector<std::unique_ptr<Body_part>> childs;

private:
    Body_part_base() = default;
    Body_part_base(const Body_part_base& other);
    Body_part_base(Body_part_base&& other) noexcept;
    Body_part_base& operator=(const Body_part_base& other);
    Body_part_base& operator=(Body_part_base&& other) noexcept;

    void swap(Body_part_base& other);

    friend class Body_part;
};

struct Body_part : Body_part_base
{
    Body_part_type type;

    Body_part(Body_part_type type);

    Body_part& addChild(Body_part part);
    std::string describe(const std::string& pronoun);
};