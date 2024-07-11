#include "entity/components/PassabilityComponent.h"

#include "entity/Entity.h"
#include "game/World.h"
#include "utils/EnumUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

const std::string PassabilityComponent::key = "passability";

PassabilityComponent::PassabilityComponent(TilePassability passability)
    : EntityComponent(key)
    , passability(passability)
{
}

void PassabilityComponent::onEntityAddedToWorld(World* world)
{
    world->addPassability(entity->getPos(), passability);
}

void PassabilityComponent::onEntityRemovedFromWorld(World* world)
{
    world->removePassability(entity->getPos(), passability);
}

TilePassability PassabilityComponent::getPassability() const
{
    return passability;
}

}  // namespace Rival
