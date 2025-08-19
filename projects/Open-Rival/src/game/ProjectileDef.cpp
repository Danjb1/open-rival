#include "game/ProjectileDef.h"

namespace Rival {

ProjectileDef ProjectileDef::fromJson(const json& j)
{
    ProjectileDef projectileDef;
    projectileDef.texture = j.at("texture");
    projectileDef.impactEffectName = j.value("impactEffect", "");
    projectileDef.numAnimFrames = j.value("animFrames", 0);
    projectileDef.shouldUseRotation = j.value("rotation", true);

    return projectileDef;
}

}  // namespace Rival
