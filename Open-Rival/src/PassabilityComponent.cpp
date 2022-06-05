#include "pch.h"
#include "PassabilityComponent.h"

#include "Entity.h"

namespace Rival {

    const std::string PassabilityComponent::key = "passability";

    PassabilityComponent::PassabilityComponent(TilePassability passability)
        : EntityComponent(key)
        , passability(passability)
    {
    }

    void PassabilityComponent::onEntitySpawned(Scenario* scenario)
    {
        scenario->setPassability(entity->getPos(), passability);
    }

    TilePassability PassabilityComponent::getPassability() const
    {
        return passability;
    }

}  // namespace Rival
