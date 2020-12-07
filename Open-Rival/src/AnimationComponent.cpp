#include "pch.h"
#include "AnimationComponent.h"

#include <stdexcept>

#include "Entity.h"
#include "Scenario.h"
#include "TimerUtils.h"
#include "Unit.h"

namespace Rival {

    const std::string AnimationComponent::key = "animation";

    AnimationComponent::AnimationComponent(
            const Animations::Animation animation)
        : EntityComponent(key),
          animation(animation),
          currentAnimFrame(0),
          msPassedCurrentAnimFrame(0) {}

    void AnimationComponent::onEntitySpawned(Scenario*) {
        spriteComponent = entity->getComponent<SpriteComponent>(
                SpriteComponent::key);
        facingComponent = entity->getComponent<FacingComponent>(
                FacingComponent::key);
        setAnimation(animation);
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

    void AnimationComponent::setAnimation(Animations::Animation newAnimation) {
        animation = newAnimation;
        msPassedCurrentAnimFrame = 0;
        setCurrentAnimFrame(0);
    }

    void AnimationComponent::setCurrentAnimFrame(int newAnimFrame) {
        currentAnimFrame = newAnimFrame;

        if (spriteComponent) {
            // Update the SpriteComponent, if present.
            // This is what actually causes the rendered image to change.
            spriteComponent->setTxIndex(getCurrentSpriteIndex());
        }
    }

    void AnimationComponent::advanceFrame(int numAnimFrames, int msPerAnimFrame) {
        int newAnimFrame = (currentAnimFrame + 1) % numAnimFrames;
        setCurrentAnimFrame(newAnimFrame);
        msPassedCurrentAnimFrame -= msPerAnimFrame;
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
        // TODO: This should vary based on a Unit's speed
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