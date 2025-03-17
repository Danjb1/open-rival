#pragma once

#include <string>

#include "entity/components/BasicAnimationComponent.h"

namespace Rival {

class EffectDef;

/**
 * Component that controls the animation of an Effect's SpriteComponent.
 */
class EffectAnimationComponent : public BasicAnimationComponent
{
public:
    EffectAnimationComponent(const EffectDef& effectDef);

    // Begin BasicAnimationComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void animationFinished() override;
    int getFacingIndex(std::shared_ptr<FacingComponent> facingComponent) const override;
    // End EntityComponent override

public:
    static const std::string key;

private:
    const EffectDef& effectDef;
};

}  // namespace Rival
