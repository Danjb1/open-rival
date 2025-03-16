#pragma once

#include <memory>
#include <string>

#include "entity/components/EntityComponent.h"
#include "entity/components/FacingComponent.h"
#include "game/Animations.h"
#include "utils/CollectionUtils.h"

namespace Rival {

class SpriteComponent;

/**
 * Component that controls the animation and facing of an Entity's SpriteComponent.
 */
class BasicAnimationComponent
    : public EntityComponent
    , public FacingListener
{

public:
    BasicAnimationComponent(const std::string& key);
    virtual ~BasicAnimationComponent() = default;

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void destroy() override;
    void update(int delta) override;
    // End EntityComponent override

    // Begin FacingListener override
    void facingChanged(Facing newFacing) override;
    // End FacingListener override

    int getCurrentSpriteIndex() const;

protected:
    void setAnimationData(const Animation* animation);
    void setCurrentAnimFrame(int newAnimFrame);
    void refreshSpriteComponent() const;
    void advanceFrame(int numAnimFrames, int msPerAnimFrame);
    virtual void animationFinished();
    int getNumAnimFrames() const;
    virtual int getMsPerAnimFrame() const;

    int getFacingOffset() const;
    int getFacingStride() const;
    virtual int getFacingIndex(std::shared_ptr<FacingComponent> facingComponent) const;

protected:
    std::weak_ptr<SpriteComponent> weakSpriteComponent;
    std::weak_ptr<FacingComponent> weakFacingComponent;

    const Animation* animation = nullptr;
    int currentAnimFrame = 0;
    int msPassedCurrentAnimFrame = 0;
};

}  // namespace Rival
