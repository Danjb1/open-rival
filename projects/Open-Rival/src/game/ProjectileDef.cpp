#include "game/ProjectileDef.h"

namespace Rival {

ProjectileDef ProjectileDef::fromJson(const json& j)
{
    const std::string texture = j.at("texture");

    ProjectileDef projectileDef(texture);
    projectileDef.numAnimFrames = j.value("animFrames", 0);
    projectileDef.shouldUseRotation = j.value("rotation", true);

    return projectileDef;
}

}  // namespace Rival
