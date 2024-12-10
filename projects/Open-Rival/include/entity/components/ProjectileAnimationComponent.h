#pragma once

#include <memory>
#include <string>

#include "entity/components/EntityComponent.h"
#include "entity/components/FacingComponent.h"
#include "game/Animations.h"
#include "game/ProjectileDef.h"
#include "utils/CollectionUtils.h"

namespace Rival {

class SpriteComponent;
struct Animation;

/**
 * Component that controls the animation of a projectile.
 */
class ProjectileAnimationComponent : public EntityComponent
{

public:
    ProjectileAnimationComponent(const ProjectileDef& projectileDef);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void update(int delta) override;
    // End EntityComponent override

    int getCurrentSpriteIndex() const;

private:
    void setCurrentAnimFrame(int newAnimFrame);
    void refreshSpriteComponent() const;
    void advanceFrame(int numAnimFrames, int msPerAnimFrame);
    int getNumAnimFrames() const;
    int getFacingOffset() const;

public:
    static const std::string key;

    // TMP: hardcoded anim rate for now
    static constexpr int msPerFrame = 30;

private:
    std::weak_ptr<FacingComponent> weakFacingComponent;
    std::weak_ptr<SpriteComponent> weakSpriteComponent;

    Animation animation;

    int currentAnimFrame = 0;
    int msPassedCurrentAnimFrame = 0;
};

}  // namespace Rival
