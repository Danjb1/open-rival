#include "commands/AttackCommand.h"

#include "entity/components/AttackComponent.h"
#include "entity/components/MovementComponent.h"
#include "game/MapUtils.h"
#include "game/Pathfinding.h"
#include "utils/BufferUtils.h"
#include "utils/LogUtils.h"

namespace Rival {

AttackCommand::AttackCommand(std::vector<int> entityIds, int targetEntityId)
    : GameCommand(GameCommandType::Attack)
    , entityIds(entityIds)
    , targetEntityId(targetEntityId)
{
}

void AttackCommand::serialize(std::vector<char>& buffer) const
{
    GameCommand::serialize(buffer);

    BufferUtils::addVectorToBuffer(buffer, entityIds);
    BufferUtils::addToBuffer(buffer, targetEntityId);
}

std::shared_ptr<AttackCommand> AttackCommand::deserialize(std::vector<char> buffer, std::size_t& offset)
{
    std::vector<int> entityIds = BufferUtils::readVectorFromBuffer<int>(buffer, offset);

    int targetEntityId;
    BufferUtils::readFromBuffer(buffer, offset, targetEntityId);

    return std::make_shared<AttackCommand>(entityIds, targetEntityId);
}

void AttackCommand::execute(GameCommandContext& context)
{
    World& world = context.getWorld();

    std::weak_ptr<Entity> targetEntityWeak = world.getMutableEntityWeak(targetEntityId);
    std::shared_ptr<Entity> targetEntity = targetEntityWeak.lock();
    if (!targetEntity)
    {
        // Target entity has been deleted since this command was issued
        return;
    }

    // TODO: Verify that target entity can be attacked

    // Set the target for our attack
    for (int entityId : entityIds)
    {
        Entity* entity = world.getMutableEntity(entityId);
        if (!entity)
        {
            // Entity has been deleted since this command was issued
            continue;
        }

        auto attackComponent = entity->getComponent<AttackComponent>(AttackComponent::key);
        if (!attackComponent)
        {
            LOG_WARN("Tried to attack with a non-attacking entity");
            continue;
        }

        attackComponent->setTarget(targetEntityWeak);
    }
}

}  // namespace Rival
