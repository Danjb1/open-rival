#pragma once

#include <string>

#include "entity/components/BasicAnimationComponent.h"
#include "game/ProjectileDef.h"
#include "utils/CollectionUtils.h"

namespace Rival {

/**
 * Component that controls the animation of a projectile.
 */
class ProjectileAnimationComponent : public BasicAnimationComponent
{

public:
    ProjectileAnimationComponent(const ProjectileDef& projectileDef);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    // End EntityComponent override

public:
    static const std::string key;

    // TMP: hardcoded anim rate for now
    static constexpr int msPerFrame = 30;

private:
    // For projecitles, the animation is stored internally.
    // TODO: Should we move this to the ProjectileDef to save memory?
    Animation projectileAnimation;
};

}  // namespace Rival
