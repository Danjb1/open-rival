#include "pch.h"

#include "BuildingAnimationComponent.h"

#include <stdexcept>

#include "Animations.h"
#include "Entity.h"
#include "Resources.h"
#include "SpriteComponent.h"
#include "TimeUtils.h"
#include "Unit.h"
#include "UnitDef.h"
#include "World.h"

namespace Rival {

const std::string BuildingAnimationComponent::key = "buildingAnimation";

BuildingAnimationComponent::BuildingAnimationComponent(const BuildingDef& buildingDef)
    : EntityComponent(key)
    , buildingDef(buildingDef)
    , animation(nullptr)
{
}

void BuildingAnimationComponent::onEntitySpawned(World*)
{
    weakSpriteComponent = entity->requireComponentWeak<SpriteComponent>(SpriteComponent::key);

    // TMP: Build immediately
    setAnimation(buildingDef.getAnimation(BuildingAnimationType::Built));
}

void BuildingAnimationComponent::update()
{
    if (!animation)
    {
        return;
    }

    int numAnimFrames = getNumAnimFrames();

    if (numAnimFrames == 1)
    {
        // Nothing to animate
        return;
    }

    msPassedCurrentAnimFrame += TimeUtils::timeStepMs;

    int msPerAnimFrame = getMsPerAnimFrame();
    if (msPassedCurrentAnimFrame >= msPerAnimFrame)
    {
        advanceFrame(numAnimFrames, msPerAnimFrame);
    }
}

void BuildingAnimationComponent::setAnimation(const Animation* newAnimation)
{
    animation = newAnimation;
    msPassedCurrentAnimFrame = 0;
    setCurrentAnimFrame(0);
}

void BuildingAnimationComponent::setCurrentAnimFrame(int newAnimFrame)
{
    currentAnimFrame = newAnimFrame;
    refreshSpriteComponent();
}

void BuildingAnimationComponent::refreshSpriteComponent() const
{
    auto spriteComponent = weakSpriteComponent.lock();
    if (!spriteComponent)
    {
        return;
    }

    // Update the SpriteComponent, if present.
    // This is what actually causes the rendered image to change.
    spriteComponent->setTxIndex(getCurrentSpriteIndex());
}

void BuildingAnimationComponent::advanceFrame(int numAnimFrames, int msPerAnimFrame)
{
    int newAnimFrame = (currentAnimFrame + 1) % numAnimFrames;
    setCurrentAnimFrame(newAnimFrame);
    msPassedCurrentAnimFrame -= msPerAnimFrame;
}

int BuildingAnimationComponent::getCurrentSpriteIndex() const
{
    if (!animation)
    {
        return 0;
    }

    return animation->startIndex + currentAnimFrame;
}

int BuildingAnimationComponent::getNumAnimFrames() const
{
    if (!animation)
    {
        return 0;
    }

    return animation->endIndex - animation->startIndex + 1;
}

int BuildingAnimationComponent::getMsPerAnimFrame() const
{
    return animation ? animation->msPerFrame : 0;
}

}  // namespace Rival
