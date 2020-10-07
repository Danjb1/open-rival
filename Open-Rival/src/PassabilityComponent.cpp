#include "pch.h"
#include "PassabilityComponent.h"

#include "Scenario.h"

namespace Rival {

    const std::string PassabilityComponent::key = "passability";

    PassabilityComponent::PassabilityComponent(TilePassability passability)
        : EntityComponent(key),
          passability(passability) {}

    void PassabilityComponent::onEntitySpawned(Scenario* scenario) {
        scenario->setPassability(entity->getX(), entity->getY(), passability);
    }

    TilePassability PassabilityComponent::getPassability() const {
        return passability;
    }

}  // namespace Rival
