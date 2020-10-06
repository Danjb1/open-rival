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
        AnimationComponent(const Animations::Animation animation);

        // Inherited from EntityComponent
        void onEntitySpawned() override;
        void update() override;

        void setAnimation(Animations::Animation newAnimation);

        int getCurrentSpriteIndex() const;

    private:
        static const std::string key;

        SpriteComponent* spriteComponent;
        FacingComponent* facingComponent;

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
