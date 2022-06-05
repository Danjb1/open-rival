#include "pch.h"
#include "MovementComponent.h"

namespace Rival {

    const std::string MovementComponent::key = "movement";

    MovementComponent::MovementComponent() : EntityComponent(key) {}

    void MovementComponent::addListener(MovementListener* listener)
    {
        if (!listener)
        {
            return;
        }
        listeners.emplace(listener);
    }

    void MovementComponent::removeListener(MovementListener* listener)
    {
        if (!listener)
        {
            return;
        }
        listeners.erase(listener);
    }

}  // namespace Rival
