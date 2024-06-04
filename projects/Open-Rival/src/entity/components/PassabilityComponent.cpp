#include "entity/components/PassabilityComponent.h"

#include "entity/Entity.h"
#include "game/World.h"

namespace Rival {

const std::string PassabilityComponent::key = "passability";

PassabilityComponent::PassabilityComponent(TilePassability passability)
    : EntityComponent(key)
    , passability(passability)
{
}

void PassabilityComponent::onEntityAddedToWorld(World* world)
{
    world->setPassability(entity->getPos(), passability);
}

void PassabilityComponent::onEntityRemovedFromWorld(World* world)
{
    world->setPassability(entity->getPos(), TilePassability::Clear);
}

TilePassability PassabilityComponent::getPassability() const
{
    return passability;
}

}  // namespace Rival
