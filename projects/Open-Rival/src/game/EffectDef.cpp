#include "game/EffectDef.h"

namespace Rival {

EffectDef EffectDef::fromJson(const json& j)
{
    EffectDef effectDef;
    effectDef.animation = tryReadAnimation(j);
    effectDef.texture = j.value("texture", "");
    effectDef.shouldUseAngle = j.value("useAngle", false);

    const std::string layerStr = j.value("layer", "");
    effectDef.layer = (layerStr == "ground") ? Layer::Ground : Layer::Overlay;

    return effectDef;
}

Animation EffectDef::tryReadAnimation(const json& j)
{
    const json animData = j.at("animation");

    const int startIndex = animData.value("startIndex", 0);
    const int endIndex = animData.value("endIndex", 0);
    const int msPerFrame = animData.value("msPerFrame", Animation::defaultMsPerFrame);
    const int facingStride = animData.value("facingStride", 0);

    return { startIndex, endIndex, msPerFrame, facingStride };
}

const Animation* EffectDef::getAnimation() const
{
    return &animation;
}

}  // namespace Rival
