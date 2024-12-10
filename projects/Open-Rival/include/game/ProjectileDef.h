#pragma once

#include <string>

#include "utils/JsonUtils.h"

namespace Rival {

class ProjectileDef final
{
public:
    static ProjectileDef fromJson(const json& j);

    ProjectileDef(std::string texture)
        : texture(texture)
    {
    }

public:
    std::string texture;
    int numAnimFrames = 0;
    bool shouldUseRotation = false;
};

}  // namespace Rival
