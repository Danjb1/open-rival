#include "entity/components/BuildingAnimationComponent.h"

namespace Rival {

const std::string BuildingAnimationComponent::key = "buildingAnimation";

BuildingAnimationComponent::BuildingAnimationComponent(const AnimationContainer<BuildingAnimationType>& animContainer)
    : BasicAnimationComponent(key)
    , animContainer(animContainer)
{
}

void BuildingAnimationComponent::onEntityFirstAddedToWorld(World* world)
{
    BasicAnimationComponent::onEntityFirstAddedToWorld(world);

    // TMP: Build immediately
    setAnimationData(animContainer.getAnimation(BuildingAnimationType::Built));
}

}  // namespace Rival
