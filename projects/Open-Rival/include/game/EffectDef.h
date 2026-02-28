#pragma once

#include <string>

#include "game/Animations.h"
#include "utils/JsonUtils.h"

namespace Rival {

class EffectDef final
{
public:
    enum Layer
    {
        // Layer used for grounded effects, e.g. corpses
        Ground,

        // Layer used for overlay effects, e.g. explosions
        Overlay
    };

    static EffectDef fromJson(const json& j);

    const Animation* getAnimation() const;

private:
    static Animation tryReadAnimation(const json& j);

public:
    /** Animation to play. */
    Animation animation;

    /** Name of the texture to use. */
    std::string texture;

    /** Layer on which the effect should be rendered. */
    Layer layer;

    /** Whether the effect supports multiple facings. */
    bool shouldUseAngle = false;
};

}  // namespace Rival
