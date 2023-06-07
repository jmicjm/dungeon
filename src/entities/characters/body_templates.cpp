#include "body_templates.h"
#include "../../components/armor.h"
#include "../../components/weapon.h"

using enum Body_part_type;

static bool verifyArmorType(const entt::registry& registry, entt::entity entity, Body_part_type bpt)
{
    if (auto armor = registry.try_get<Armor>(entity))
    {
        return armor->body_part_type == bpt;
    }
    return false;
}

Inventory createHeadInventory()
{
    return Inventory{ 1, [](const entt::registry& registry, entt::entity entity, const Inventory& inventory) {
        return verifyArmorType(registry, entity, Body_part_type::HEAD);
    } };
}

Inventory createUpperTorsoInventory()
{
    return Inventory{ 1, [](const entt::registry& registry, entt::entity entity, const Inventory& inventory) {
        return verifyArmorType(registry, entity, Body_part_type::UPPER_TORSO);
    } };
}

Inventory createLowerTorsoInventory()
{
    return Inventory{ 1, [](const entt::registry& registry, entt::entity entity, const Inventory& inventory) {
        return verifyArmorType(registry, entity, Body_part_type::LOWER_TORSO);
    } };
}

Inventory createFootInventory()
{
    return Inventory{ 1, [](const entt::registry& registry, entt::entity entity, const Inventory& inventory) {
        return verifyArmorType(registry, entity, Body_part_type::FOOT);
    } };
}

Inventory createHandInventory()
{
    return Inventory{ 2, [](const entt::registry& registry, entt::entity entity, const Inventory& inventory) {
        if (verifyArmorType(registry, entity, Body_part_type::HAND))
        {
            if (countTInInventory<Armor>(registry, inventory) == 0) return true;
        }
        else if (auto weapon = registry.try_get<Weapon>(entity))
        {
            if (countTInInventory<Weapon>(registry, inventory) == 0) return true;
        }
        return false;
    } };
}

Body createHumanoidBody(entt::registry& registry)
{
    Body body;
    body.graph.root = std::make_unique<Body_part>(Body_part{ HEAD, registry, createHeadInventory() }
        .addChild(Body_part{ NECK }
            .addChild(Body_part{ UPPER_TORSO, registry, createUpperTorsoInventory() }
                .addChild(createArm(registry, "left"))
                .addChild(createArm(registry, "right"))
                .addChild(Body_part{ LOWER_TORSO, registry, createLowerTorsoInventory() }
                    .addChild(createLeg(registry, "left"))
                    .addChild(createLeg(registry, "right"))
                )
            )
        )
    );

    return body;
}

Body_part createArm(entt::registry& registry, const std::string& desc)
{
    return Body_part{ JOINT, desc + " shoulder", true}
        .addChild(Body_part{ FOREARM, desc }
            .addChild(Body_part{ JOINT, desc + " elbow", true }
                .addChild(Body_part{ ARM, desc }
                    .addChild(Body_part{ JOINT, desc + " wrist", true }
                        .addChild(Body_part{ HAND, desc, false, registry, createHandInventory()})
                    )
                )
            )
        );
}

Body_part createLeg(entt::registry& registry, const std::string& desc)
{
    return Body_part{ JOINT, desc + " hip", true }
        .addChild(Body_part{ THIGH, desc }
            .addChild(Body_part{ JOINT, desc + " knee", true }
                .addChild(Body_part{ CALF, desc }
                    .addChild(Body_part{ JOINT, desc + " ankle", true }
                        .addChild(Body_part{ FOOT, desc, false, registry, createFootInventory()})
                    )
                )
            )
        );
}
