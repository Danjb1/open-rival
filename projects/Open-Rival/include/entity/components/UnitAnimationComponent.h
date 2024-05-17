#pragma once

#include <memory>
#include <string>

#include "entity/Unit.h"
#include "entity/components/EntityComponent.h"
#include "entity/components/FacingComponent.h"

namespace Rival {

class SpriteComponent;
class UnitDef;
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
    UnitAnimationComponent(const UnitDef& unitDef);

    // Begin EntityComponent override
    virtual void onEntityAddedToWorld(World* world) override;
    virtual void destroy() override;
    virtual void update() override;
    // End EntityComponent override

    // Begin UnitStateListener override
    virtual void onUnitStateChanged(const UnitState newState) override;
    // End UnitStateListener override

    // Begin FacingListener override
    void facingChanged(Facing newFacing) override;
    // End FacingListener override

    void setAnimation(const Animation* newAnimation);

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

    const UnitDef& unitDef;

    const Animation* animation;

    int currentAnimFrame = 0;

    int msPassedCurrentAnimFrame = 0;
};

}  // namespace Rival
