#pragma once

#include <memory>
#include <string>

#include "entity/Unit.h"
#include "entity/components/EntityComponent.h"
#include "entity/components/FacingComponent.h"
#include "game/Animations.h"
#include "utils/CollectionUtils.h"

namespace Rival {

class SpriteComponent;
class AnimationContainer;
struct Animation;

/** Interface that allows a class to be notified of animation events. */
class AnimationListener
{
public:
    virtual void onAnimationFinished(UnitAnimationType animType) = 0;
};

/**
 * Component that controls the animation and facing of a unit's SpriteComponent.
 */
class UnitAnimationComponent
    : public EntityComponent
    , public UnitStateListener
    , public FacingListener
{

public:
    UnitAnimationComponent(const AnimationContainer& unitDef);

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

    void addListener(std::weak_ptr<AnimationListener> listener);
    void removeListener(std::weak_ptr<AnimationListener> listener);

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

    WeakPtrSet<AnimationListener> listeners;

    const AnimationContainer& animationContainer;

    const Animation* animation;
    UnitAnimationType currentAnimType = UnitAnimationType::Uninitialized;

    int currentAnimFrame = 0;

    int msPassedCurrentAnimFrame = 0;
};

}  // namespace Rival
