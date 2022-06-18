#include "pch.h"

#include "PassabilityComponent.h"

#include "Entity.h"
#include "World.h"

namespace Rival {

const std::string PassabilityComponent::key = "passability";

PassabilityComponent::PassabilityComponent(TilePassability passability)
    : EntityComponent(key)
    , passability(passability)
{
}

void PassabilityComponent::onEntitySpawned(World* world)
{
    world->setPassability(entity->getPos(), passability);
}

TilePassability PassabilityComponent::getPassability() const
{
    return passability;
}

}  // namespace Rival
