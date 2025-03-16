#pragma once

#include <string>

#include "entity/components/BasicAnimationComponent.h"

namespace Rival {

/**
 * Component that controls the animation of a building's SpriteComponent.
 */
class BuildingAnimationComponent : public BasicAnimationComponent
{

public:
    BuildingAnimationComponent(const AnimationContainer<BuildingAnimationType>& animContainer);

    // Begin BasicAnimationComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    // End EntityComponent override

public:
    static const std::string key;

private:
    const AnimationContainer<BuildingAnimationType>& animContainer;
};

}  // namespace Rival
