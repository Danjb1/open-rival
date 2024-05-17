#pragma once

#include <memory>
#include <string>

#include "entity/components/EntityComponent.h"
#include "entity/components/FacingComponent.h"

namespace Rival {

class SpriteComponent;
class BuildingDef;
struct Animation;

/**
 * Component that controls the animation of a building's SpriteComponent.
 */
class BuildingAnimationComponent : public EntityComponent
{

public:
    BuildingAnimationComponent(const BuildingDef& buildingDef);

    // Begin EntityComponent override
    virtual void onEntityAddedToWorld(World* world) override;
    virtual void update() override;
    // End EntityComponent override

    void setAnimation(const Animation* newAnimation);

    int getCurrentSpriteIndex() const;

private:
    void setCurrentAnimFrame(int newAnimFrame);
    void refreshSpriteComponent() const;
    void advanceFrame(int numAnimFrames, int msPerAnimFrame);
    int getNumAnimFrames() const;
    int getMsPerAnimFrame() const;

public:
    static const std::string key;

private:
    std::weak_ptr<SpriteComponent> weakSpriteComponent;

    const BuildingDef& buildingDef;

    const Animation* animation;

    int currentAnimFrame = 0;

    int msPassedCurrentAnimFrame = 0;
};

}  // namespace Rival
