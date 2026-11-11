#include "entity/components/PassabilityComponent.h"

#include "entity/Entity.h"
#include "game/MapUtils.h"
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
    PassabilityMap* passabilityMap = world->getPassabilityMap();
    passabilityMap->addPassability(entity->getPos(), passability);

    if (entity->isBig())
    {
        const auto northEast = MapUtils::getNeighbor(entity->getPos(), Facing::NorthEast, *passabilityMap);
        const auto north = MapUtils::getNeighbor(entity->getPos(), Facing::North, *passabilityMap);
        const auto northWest = MapUtils::getNeighbor(entity->getPos(), Facing::NorthWest, *passabilityMap);

        if (!(northEast && north && northWest))
        {
            throw std::runtime_error("Invalid entity placement!");
        }

        passabilityMap->addPassability(*northEast, passability);
        passabilityMap->addPassability(*north, passability);
        passabilityMap->addPassability(*northWest, passability);
    }
}

void PassabilityComponent::onEntityRemovedFromWorld(World* world)
{
    PassabilityMap* passabilityMap = world->getPassabilityMap();
    passabilityMap->removePassability(entity->getPos(), passability);

    if (entity->isBig())
    {
        const auto northEast = MapUtils::getNeighbor(entity->getPos(), Facing::NorthEast, *passabilityMap);
        const auto north = MapUtils::getNeighbor(entity->getPos(), Facing::North, *passabilityMap);
        const auto northWest = MapUtils::getNeighbor(entity->getPos(), Facing::NorthWest, *passabilityMap);
        passabilityMap->removePassability(*northEast, passability);
        passabilityMap->removePassability(*north, passability);
        passabilityMap->removePassability(*northWest, passability);
    }
}

TilePassability PassabilityComponent::getPassability() const
{
    return passability;
}

}  // namespace Rival
