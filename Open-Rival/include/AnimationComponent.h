#pragma once

#include <memory>

#include "Animations.h"
#include "EntityComponent.h"
#include "FacingComponent.h"
#include "SpriteComponent.h"
#include "UnitPropsComponent.h"

namespace Rival {

/**
 * Component that controls the animation and facing of a SpriteComponent.
 */
class AnimationComponent
    : public EntityComponent
    , public UnitStateListener
    , public FacingListener
{

public:
    AnimationComponent(const Animations::Animation animation);

    // Begin EntityComponent override
    virtual void onEntitySpawned(World* world) override;
    virtual void onDelete() override;
    virtual void update() override;
    // End EntityComponent override

    // Begin UnitStateListener override
    virtual void onUnitStateChanged(const UnitState newState) override;
    // End UnitStateListener override

    // Begin FacingListener override
    void facingChanged(Facing newFacing) override;
    // End FacingListener override

    void setAnimation(Animations::Animation newAnimation);

    int getCurrentSpriteIndex() const;

private:
    void setCurrentAnimFrame(int newAnimFrame);

    void refreshSpriteComponent() const;

    void advanceFrame(int numAnimFrames, int msPerAnimFrame);

    int getNumAnimFrames() const;

    int getMsPerAnimFrame() const;

    int getFacingOffset() const;

public:
    static const std::string key;

private:
    std::weak_ptr<UnitPropsComponent> weakUnitPropsComponent;
    std::weak_ptr<SpriteComponent> weakSpriteComponent;
    std::weak_ptr<FacingComponent> weakFacingComponent;

    Animations::Animation animation;

    int currentAnimFrame;

    int msPassedCurrentAnimFrame;
};
}  // namespace Rival
