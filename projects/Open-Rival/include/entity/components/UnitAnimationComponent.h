#pragma once

#include <memory>
#include <string>

#include "entity/Unit.h"
#include "entity/components/EntityComponent.h"
#include "entity/components/FacingComponent.h"
#include "game/Animations.h"
#include "utils/CollectionUtils.h"

namespace Rival {

template class AnimationListener<UnitAnimationType>;
class SpriteComponent;
struct Animation;

/**
 * Component that controls the animation and facing of a unit's SpriteComponent.
 */
class UnitAnimationComponent
    : public EntityComponent
    , public UnitStateListener
    , public FacingListener
{

public:
    UnitAnimationComponent(const AnimationContainer<UnitAnimationType>& animContainer);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void destroy() override;
    void update(int delta) override;
    // End EntityComponent override

    // Begin UnitStateListener override
    void onUnitStateChanged(const UnitState newState) override;
    // End UnitStateListener override

    // Begin FacingListener override
    void facingChanged(Facing newFacing) override;
    // End FacingListener override

    void addListener(std::weak_ptr<AnimationListener<UnitAnimationType>> listener);
    void removeListener(std::weak_ptr<AnimationListener<UnitAnimationType>> listener);

    void setAnimation(UnitAnimationType animType);

    int getCurrentSpriteIndex() const;

private:
    void setCurrentAnimFrame(int newAnimFrame);
    void refreshSpriteComponent() const;
    void advanceFrame(int numAnimFrames, int msPerAnimFrame);
    int getNumAnimFrames() const;
    int getMsPerAnimFrame() const;
    int getFacingStride() const;
    int getFacingOffset() const;

public:
    static const std::string key;

private:
    std::weak_ptr<SpriteComponent> weakSpriteComponent;
    std::weak_ptr<FacingComponent> weakFacingComponent;

    WeakPtrSet<AnimationListener<UnitAnimationType>> listeners;

    const AnimationContainer<UnitAnimationType>& animContainer;

    const Animation* animation;
    UnitAnimationType currentAnimType = UnitAnimationType::Uninitialized;

    int currentAnimFrame = 0;

    int msPassedCurrentAnimFrame = 0;
};

}  // namespace Rival
