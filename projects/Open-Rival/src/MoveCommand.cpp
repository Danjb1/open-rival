#include "pch.h"

#include "MoveCommand.h"

#include "utils/BufferUtils.h"
#include "MapUtils.h"
#include "MovementComponent.h"

namespace Rival {

MoveCommand::MoveCommand(int entityId, MapNode destination)
    : GameCommand(GameCommandType::Move)
    , entityId(entityId)
    , destination(destination)
{
}

void MoveCommand::serialize(std::vector<char>& buffer) const
{
    GameCommand::serialize(buffer);

    BufferUtils::addToBuffer(buffer, entityId);
    BufferUtils::addToBuffer(buffer, destination);
}

std::shared_ptr<MoveCommand> MoveCommand::deserialize(std::vector<char> buffer, size_t& offset)
{
    int entityId;
    BufferUtils::readFromBuffer(buffer, offset, entityId);

    MapNode destination;
    BufferUtils::readFromBuffer(buffer, offset, destination);

    return std::make_shared<MoveCommand>(entityId, destination);
}

void MoveCommand::execute(GameCommandContext& context)
{
    World& world = context.getWorld();

    Entity* entity = world.getMutableEntity(entityId);
    if (!entity)
    {
        // Entity has been deleted since this command was issued
        return;
    }

    auto moveComponent = entity->getComponent<MovementComponent>(MovementComponent::key);
    if (!moveComponent)
    {
        std::cerr << "Tried to move an immovable entity\n";
        return;
    }

    moveComponent->moveTo(destination);
}

}  // namespace Rival
