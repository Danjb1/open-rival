#include "game/ProjectileDef.h"

namespace Rival {

ProjectileDef ProjectileDef::fromJson(const json& j)
{
    const std::string texture = j.at("texture");

    ProjectileDef projectileDef(texture);
    projectileDef.animFrames = j.value("animFrames", 0);
    projectileDef.useRotation = j.value("rotation", false);

    return projectileDef;
}

}  // namespace Rival
