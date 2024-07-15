#include "commands/MoveCommand.h"

#include "entity/components/AttackComponent.h"
#include "entity/components/MovementComponent.h"
#include "game/MapUtils.h"
#include "game/Pathfinding.h"
#include "utils/BufferUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

MoveCommand::MoveCommand(std::vector<int> entityIds, MapNode destination)
    : GameCommand(GameCommandType::Move)
    , entityIds(entityIds)
    , destination(destination)
{
}

void MoveCommand::serialize(std::vector<char>& buffer) const
{
    GameCommand::serialize(buffer);

    BufferUtils::addVectorToBuffer(buffer, entityIds);
    BufferUtils::addToBuffer(buffer, destination);
}

std::shared_ptr<MoveCommand> MoveCommand::deserialize(std::vector<char> buffer, std::size_t& offset)
{
    std::vector<int> entityIds = BufferUtils::readVectorFromBuffer<int>(buffer, offset);

    MapNode destination;
    BufferUtils::readFromBuffer(buffer, offset, destination);

    return std::make_shared<MoveCommand>(entityIds, destination);
}

void MoveCommand::execute(GameCommandContext& context)
{
    World& world = context.getWorld();

    // Prepare all entities for movement.
    // This needs to be called for all entities *before* any are moved, so that we can take measures to prevent them
    // from obstructing each other.
    int numValidEntities = 0;
    for (int entityId : entityIds)
    {
        Entity* entity = world.getMutableEntity(entityId);
        if (!entity)
        {
            // Entity has been deleted since this command was issued
            continue;
        }

        auto moveComponent = entity->getComponent<MovementComponent>(MovementComponent::key);
        if (!moveComponent)
        {
            LOG_WARN("Tried to move an immovable entity");
            continue;
        }

        moveComponent->prepareForMovement();
        ++numValidEntities;
    }

    // Create a context to make group pathfinding more efficient
    Pathfinding::Context pathfindingContext(numValidEntities);

    // Move entities
    for (int entityId : entityIds)
    {
        Entity* entity = world.getMutableEntity(entityId);
        if (!entity)
        {
            // Entity has been deleted since this command was issued
            continue;
        }

        auto moveComponent = entity->getComponent<MovementComponent>(MovementComponent::key);
        if (!moveComponent)
        {
            LOG_WARN("Tried to move an immovable entity");
            continue;
        }

        if (auto attackComponent = entity->getComponent<AttackComponent>(AttackComponent::key))
        {
            // Clear current target when a MoveCommand is issued
            attackComponent->setTarget({});
        }

        moveComponent->moveTo(destination, pathfindingContext);
    }
}

}  // namespace Rival
