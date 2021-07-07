#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "Animations.h"
#include "EntityComponent.h"
#include "FacingComponent.h"
#include "SpriteComponent.h"

namespace Rival {

    /**
     * Component that animates a SpriteComponent.
     */
    class AnimationComponent : public EntityComponent {

    public:
        static const std::string key;

        AnimationComponent(const Animations::Animation animation);

        void onEntitySpawned(Scenario* scenario) override;

        void update() override;

        void setAnimation(Animations::Animation newAnimation);

        int getCurrentSpriteIndex() const;

    private:
        SpriteComponent* spriteComponent { nullptr };
        FacingComponent* facingComponent { nullptr };

        Animations::Animation animation;

        int currentAnimFrame;

        int msPassedCurrentAnimFrame;

        void setCurrentAnimFrame(int newAnimFrame);

        void advanceFrame(int numAnimFrames, int msPerAnimFrame);

        int getNumAnimFrames() const;

        int getMsPerAnimFrame() const;

        int getFacingOffset() const;
    };
}  // namespace Rival

#endif  // ANIMATION_COMPONENT_H
