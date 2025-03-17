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

std::shared_ptr<Entity> DeathEffectComponent::spawnEffect(Facing facing)
{
    // Create the effect
    std::shared_ptr<Entity> effect = entityFactory->createEffect(effectDef, facing);

    // Spawn it in the world
    World* world = entity->getWorld();
    world->addEntity(effect, entity->getPos());

    return effect;
}

}  // namespace Rival
