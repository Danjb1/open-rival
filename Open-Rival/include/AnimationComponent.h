#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "Animations.h"
#include "EntityComponent.h"
#include "FacingComponent.h"
#include "SpriteComponent.h"

namespace Rival {

    /**
     * Component that controls the animation and facing of a SpriteComponent.
     */
    class AnimationComponent : public EntityComponent,
                               public FacingListener {

    public:
        static const std::string key;

        AnimationComponent(const Animations::Animation animation);

        // Begin EntityComponent override
        void onEntitySpawned(Scenario* scenario) override;
        void update() override;
        // End EntityComponent override

        // Begin FacingListener override
        void facingChanged(Facing newFacing) override;
        // End FacingListener override

        void setAnimation(Animations::Animation newAnimation);

        int getCurrentSpriteIndex() const;

    private:
        SpriteComponent* spriteComponent { nullptr };
        FacingComponent* facingComponent { nullptr };

        Animations::Animation animation;

        int currentAnimFrame;

        int msPassedCurrentAnimFrame;

        void setCurrentAnimFrame(int newAnimFrame);

        void refreshSpriteComponent() const;

        void advanceFrame(int numAnimFrames, int msPerAnimFrame);

        int getNumAnimFrames() const;

        int getMsPerAnimFrame() const;

        int getFacingOffset() const;
    };
}  // namespace Rival

#endif  // ANIMATION_COMPONENT_H
