#include "body_templates.h"

using enum Body_part_type;

Body createHumanoidBody()
{
    Body body;
    body.graph.root = std::make_unique<Body_part>(Body_part{ HEAD }
        .addChild(Body_part{ UPPER_TORSO }
            .addChild(createArm("left"))
            .addChild(createArm("right"))
            .addChild(Body_part{ LOWER_TORSO }
                .addChild(createLeg("left"))
                .addChild(createLeg("right"))
            )
        )
    );

    return body;
}

Body_part createArm(const std::string& desc)
{
    return Body_part{ JOINT, desc + " shoulder", true}
        .addChild(Body_part{ FOREARM, desc }
            .addChild(Body_part{ JOINT, desc + " elbow", true }
                .addChild(Body_part{ ARM, desc }
                    .addChild(Body_part{ JOINT, desc + " wrist", true }
                        .addChild(Body_part{ HAND, desc })
                    )
                )
            )
        );
}

Body_part createLeg(const std::string& desc)
{
    return Body_part{ JOINT, desc + " hip", true }
        .addChild(Body_part{ THIGH, desc }
            .addChild(Body_part{ JOINT, desc + " knee", true }
                .addChild(Body_part{ CALF, desc }
                    .addChild(Body_part{ JOINT, desc + " ankle", true }
                        .addChild(Body_part{ FOOT, desc })
                    )
                )
            )
        );
}
