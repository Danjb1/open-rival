#pragma once

#include <string>

#include "utils/JsonFwd.h"

namespace Rival {

class ProjectileDef final
{
public:
    static ProjectileDef fromJson(const json& j);

private:
    ProjectileDef() {}

public:
    std::string texture;
    std::string impactEffectName;
    int numAnimFrames = 0;
    bool shouldUseRotation = false;
};

}  // namespace Rival
