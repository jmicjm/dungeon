#pragma once
#include "../../components/body/body.h"
#include <string>


Body createHumanoidBody();
Body_part createArm(const std::string& desc);
Body_part createLeg(const std::string& desc);
