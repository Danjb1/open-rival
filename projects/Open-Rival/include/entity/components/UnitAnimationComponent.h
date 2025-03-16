#pragma once

#include <string>

#include "entity/Unit.h"
#include "entity/components/BasicAnimationComponent.h"
#include "utils/CollectionUtils.h"

namespace Rival {

template class AnimationListener<UnitAnimationType>;

/**
 * Component that controls the animation and facing of a unit's SpriteComponent.
 */
class UnitAnimationComponent
    : public BasicAnimationComponent
    , public UnitStateListener
{

public:
    UnitAnimationComponent(const AnimationContainer<UnitAnimationType>& animContainer);

    // Begin BasicAnimationComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void destroy() override;
    void animationFinished() override;
    int getMsPerAnimFrame() const override;
    int getFacingIndex(std::shared_ptr<FacingComponent> facingComponent) const override;
    // End EntityComponent override

    // Begin UnitStateListener override
    void onUnitStateChanged(const UnitState newState) override;
    // End UnitStateListener override

    void addListener(std::weak_ptr<AnimationListener<UnitAnimationType>> listener);
    void removeListener(std::weak_ptr<AnimationListener<UnitAnimationType>> listener);

    void setAnimation(UnitAnimationType animType);
    bool hasAnimation(UnitAnimationType animType) const;

public:
    static const std::string key;

private:
    WeakPtrSet<AnimationListener<UnitAnimationType>> listeners;

    const AnimationContainer<UnitAnimationType>& animContainer;

    UnitAnimationType currentAnimType = UnitAnimationType::Uninitialized;
};

}  // namespace Rival
