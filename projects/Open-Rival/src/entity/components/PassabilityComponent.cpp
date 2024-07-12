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
    world->addPassability(entity->getPos(), passability);

    if (entity->isBig())
    {
        const auto northEast = MapUtils::getNeighbor(entity->getPos(), Facing::NorthEast, *world);
        const auto north = MapUtils::getNeighbor(entity->getPos(), Facing::North, *world);
        const auto northWest = MapUtils::getNeighbor(entity->getPos(), Facing::NorthWest, *world);

        if (!(northEast && north && northWest))
        {
            throw std::runtime_error("Invalid entity placement!");
        }

        world->addPassability(*northEast, passability);
        world->addPassability(*north, passability);
        world->addPassability(*northWest, passability);
    }
}

void PassabilityComponent::onEntityRemovedFromWorld(World* world)
{
    world->removePassability(entity->getPos(), passability);

    if (entity->isBig())
    {
        const auto northEast = MapUtils::getNeighbor(entity->getPos(), Facing::NorthEast, *world);
        const auto north = MapUtils::getNeighbor(entity->getPos(), Facing::North, *world);
        const auto northWest = MapUtils::getNeighbor(entity->getPos(), Facing::NorthWest, *world);
        world->removePassability(*northEast, passability);
        world->removePassability(*north, passability);
        world->removePassability(*northWest, passability);
    }
}

TilePassability PassabilityComponent::getPassability() const
{
    return passability;
}

}  // namespace Rival
