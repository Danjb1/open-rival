#include "pch.h"

#include "MoveCommand.h"

#include "MapUtils.h"
#include "MovementComponent.h"

namespace Rival {

MoveCommand::MoveCommand(int entityId, MapNode destination)
    : entityId(entityId)
    , destination(destination)
{
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
