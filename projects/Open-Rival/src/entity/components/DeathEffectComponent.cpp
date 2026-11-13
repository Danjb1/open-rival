#include "entity/components/DeathEffectComponent.h"

#include "entity/Entity.h"
#include "entity/EntityFactory.h"
#include "game/World.h"

namespace Rival {

const std::string DeathEffectComponent::key = "deathEffect";

DeathEffectComponent::DeathEffectComponent(
        const EffectDef& effectDef, std::shared_ptr<const EntityFactory> entityFactory)
    : EntityComponent(key)
    , effectDef(effectDef)
    , entityFactory(entityFactory)
{
}

std::shared_ptr<Entity> DeathEffectComponent::spawnEffect(Facing facing, float offsetY)
{
    // Create the effect
    std::shared_ptr<Entity> effect = entityFactory->createEffect(effectDef, facing, offsetY);

    // Spawn it in the world
    World* world = entity->getWorld();
    const MapNode& pos = entity->getPos();
    world->requestAddEntity(effect, pos.x, pos.y);

    return effect;
}

}  // namespace Rival
