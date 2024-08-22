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
    int animFrames = 0;
    bool useRotation = false;
};

}  // namespace Rival
