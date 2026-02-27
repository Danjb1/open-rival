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
    , targetTile(MapNode::Invalid)
{
}

AttackCommand::AttackCommand(std::vector<int> entityIds, MapNode targetTile)
    : GameCommand(GameCommandType::Attack)
    , entityIds(entityIds)
    , targetEntityId(-1)
    , targetTile(targetTile)
{
}

void AttackCommand::serialize(std::vector<char>& buffer) const
{
    GameCommand::serialize(buffer);

    BufferUtils::addVectorToBuffer(buffer, entityIds);
    BufferUtils::addToBuffer(buffer, targetEntityId);
    BufferUtils::addToBuffer(buffer, targetTile);
}

std::shared_ptr<AttackCommand> AttackCommand::deserialize(std::vector<char> buffer, std::size_t& offset)
{
    std::vector<int> entityIds = BufferUtils::readVectorFromBuffer<int>(buffer, offset);

    int targetEntityId;
    BufferUtils::readFromBuffer(buffer, offset, targetEntityId);

    MapNode targetTile;
    BufferUtils::readFromBuffer(buffer, offset, targetTile);

    if (targetEntityId >= 0)
    {
        return std::make_shared<AttackCommand>(entityIds, targetEntityId);
    }

    return std::make_shared<AttackCommand>(entityIds, targetTile);
}

void AttackCommand::execute(GameCommandContext& context)
{
    World& world = context.getWorld();

    // Validate the target entity
    std::weak_ptr<Entity> targetEntityWeak;
    if (targetEntityId >= 0)
    {
        targetEntityWeak = world.getMutableEntityWeak(targetEntityId);
        std::shared_ptr<Entity> targetEntity = targetEntityWeak.lock();
        if (!targetEntity)
        {
            // Target entity has been deleted since this command was issued
            return;
        }
    }

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

        if (targetEntityId >= 0)
        {
            attackComponent->setTargetEntity(targetEntityWeak);
        }
        else
        {
            attackComponent->setTargetTile(targetTile);
        }
    }
}

}  // namespace Rival
