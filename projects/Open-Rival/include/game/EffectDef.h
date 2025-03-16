#pragma once

#include <string>

#include "game/Animations.h"
#include "utils/JsonUtils.h"

namespace Rival {

class EffectDef final : public AnimationContainer<EffectAnimationType>
{
public:
    static EffectDef fromJson(const json& j);

    // Begin AnimationContainer override
    const Animation* getAnimation(EffectAnimationType animType) const override;
    // End AnimationContainer override

private:
    static Animation tryReadAnimation(const json& j);

public:
    /** Animation to play. */
    Animation animation;

    /** Name of the texture to use. */
    std::string texture;

    /** Whether the effect supports multiple facings. */
    bool shouldUseAngle = false;
};

}  // namespace Rival
