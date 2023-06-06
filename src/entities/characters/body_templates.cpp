#include "body_templates.h"
#include "../../components/inventory.h"

using enum Body_part_type;

Body createHumanoidBody(entt::registry& registry)
{
    Body body;
    body.graph.root = std::make_unique<Body_part>(Body_part{ HEAD, registry, Inventory{ 1 } }
        .addChild(Body_part{ UPPER_TORSO, registry, Inventory{ 1 } }
            .addChild(createArm(registry, "left"))
            .addChild(createArm(registry, "right"))
            .addChild(Body_part{ LOWER_TORSO, registry, Inventory{ 1 } }
                .addChild(createLeg(registry, "left"))
                .addChild(createLeg(registry, "right"))
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
                        .addChild(Body_part{ HAND, desc, false, registry, Inventory{ 1 } })
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
                        .addChild(Body_part{ FOOT, desc, false, registry, Inventory{ 1 } })
                    )
                )
            )
        );
}
