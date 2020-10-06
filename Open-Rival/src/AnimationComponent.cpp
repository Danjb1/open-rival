#include "pch.h"
#include "AnimationComponent.h"

#include <stdexcept>

#include "Entity.h"
#include "TimerUtils.h"
#include "Unit.h"

namespace Rival {

    const std::string AnimationComponent::key = "animation";

    AnimationComponent::AnimationComponent(const Animation animation)
        : EntityComponent(key),
          animation(animation),
          msPassedCurrentAnimFrame(0) {}

    void AnimationComponent::onEntitySpawned() {
        spriteComponent = dynamic_cast<SpriteComponent*>(
                entity->getComponent(SpriteComponent::key));
        facingComponent = dynamic_cast<FacingComponent*>(
                entity->getComponent(FacingComponent::key));
    }

    void AnimationComponent::update() {
        int numAnimFrames = getNumAnimFrames();

        if (numAnimFrames == 1) {
            // Nothing to animate
            return;
        }

        msPassedCurrentAnimFrame += TimerUtils::timeStepMs;

        int msPerAnimFrame = getMsPerAnimFrame();
        if (msPassedCurrentAnimFrame >= msPerAnimFrame) {
            advanceFrame(numAnimFrames, msPerAnimFrame);
        }
    }

    void AnimationComponent::advanceFrame(int numAnimFrames, int msPerAnimFrame) {
        currentAnimFrame = (currentAnimFrame + 1) % numAnimFrames;
        msPassedCurrentAnimFrame -= msPerAnimFrame;
        if (spriteComponent) {
            // TODO: Move this to a setter for currentAnimFrame
            spriteComponent->txIndex = currentAnimFrame;
        }
    }

    int AnimationComponent::getCurrentSpriteIndex() const {
        return animation.startIndex
                + getFacingOffset()
                + currentAnimFrame;
    }

    int AnimationComponent::getNumAnimFrames() const {
        return animation.endIndex - animation.startIndex + 1;
    }

    int AnimationComponent::getMsPerAnimFrame() const {
        // TODO: This should vary based on the unit's speed
        return animation.msPerFrame;
    }

    int AnimationComponent::getFacingOffset() const {
        if (!facingComponent) {
            return 0;
        }
        int numAnimFrames = getNumAnimFrames();
        int facingIndex = static_cast<int>(facingComponent->facing)
                - static_cast<int>(Facing::South);
        return facingIndex * numAnimFrames;
    }

}  // namespace Rival
