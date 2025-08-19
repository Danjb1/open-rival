#include "entity/components/ProjectileMovementComponent.h"

#include "application/Resources.h"
#include "audio/AudioSystem.h"
#include "audio/SoundSource.h"
#include "entity/EntityFactory.h"
#include "entity/Projectile.h"
#include "entity/components/HealthComponent.h"
#include "game/AttackUtils.h"
#include "game/World.h"
#include "gfx/RenderUtils.h"
#include "utils/LogUtils.h"
#include "utils/MathUtils.h"

namespace Rival {

const std::string ProjectileMovementComponent::key = "projectile_movement";

ProjectileMovementComponent::ProjectileMovementComponent(const AudioStore& audioStore,
        AudioSystem& audioSystem,
        MapNode target,
        const EffectDef* impactEffectDef,
        std::shared_ptr<std::mt19937> randomizer,
        std::shared_ptr<const EntityFactory> entityFactory)
    : EntityComponent(key)
    , audioStore(audioStore)
    , audioSystem(audioSystem)
    , target(target)
    , impactEffectDef(impactEffectDef)
    , randomizer(randomizer)
    , entityFactory(entityFactory)
{
}

void ProjectileMovementComponent::onEntityFirstAddedToWorld(World* world)
{
    entityContainer = world;

    Projectile* projectile = entity->as<Projectile>();
    if (!projectile)
    {
        LOG_WARN("ProjectileMovementComponent must be attached to a Projectile!");
        return;
    }

    // Calculate the travel distance.
    // We subtract 1 because projectile graphics are designed to finish in the tile BEFORE the destination.
    const AttackDef* attackDef = projectile->getAttackDef();
    const MapNode pos = projectile->getPos();
    const int distance = MapUtils::getDistance(pos, target) - 1;

    // This is used to ensure the projectile *visually* stops at the right place
    visualDistanceMultiplier = static_cast<float>(distance) / static_cast<float>(distance + 1);

    if (distance == 0)
    {
        // Projectile has arrived immediately, i.e. target is in an adjacent tile.
        onProjectileArrived();
        return;
    }

    // Calculate projectile lifetime based on speed
    const float speed = attackDef->projectileSpeed * 0.001f;  // TMP speed modifier
    lifetime = static_cast<int>(distance / speed);
}

void ProjectileMovementComponent::update(int delta)
{
    timeElapsed += delta;
    if (timeElapsed >= lifetime)
    {
        onProjectileArrived();
        return;
    }

    entity->moved = true;
}

void ProjectileMovementComponent::onProjectileArrived()
{
    Projectile* projectile = entity->as<Projectile>();
    const AttackDef* attackDef = projectile->getAttackDef();

    bool wasAnyTargetHit = tryDamageEntityAtTarget(attackDef);
    wasAnyTargetHit |= tryApplySplashDamage(attackDef);

    spawnImpactEffect();

    if (wasAnyTargetHit)
    {
        playImpactSound(attackDef->impactSound);
    }

    entity->markForDeletion();
}

bool ProjectileMovementComponent::tryApplySplashDamage(const AttackDef* attackDef) const
{
    bool wasAnyEntityDamaged = false;
    if (attackDef->hasSplashDamage)
    {
        SharedMutableEntityList splashTargets = entityContainer->getMutableEntitiesInRadius(target, 1);
        for (std::shared_ptr<Entity> splashTarget : splashTargets)
        {
            wasAnyEntityDamaged |=
                    AttackUtils::tryApplyAttack(*attackDef, *splashTarget, *randomizer, splashDamageMultiplier);
        }
    }
    return wasAnyEntityDamaged;
}

bool ProjectileMovementComponent::tryDamageEntityAtTarget(const AttackDef* attackDef) const
{
    std::shared_ptr<Entity> targetEntity = entityContainer->getMutableEntityAt(target);
    if (targetEntity)
    {
        return AttackUtils::tryApplyAttack(*attackDef, *targetEntity, *randomizer);
    }
    return false;
}

void ProjectileMovementComponent::spawnImpactEffect() const
{
    // Facing is irrelevant!
    std::shared_ptr<Entity> effect = entityFactory->createEffect(*impactEffectDef, Facing::North);
    World* world = entity->getWorld();
    world->addEntity(effect, target);
}

void ProjectileMovementComponent::playImpactSound(const std::string& soundName) const
{
    const SoundBank* soundBank = audioStore.getSoundBank(soundName);
    if (soundBank)
    {
        int soundId = soundBank->getRandomSound();
        audioSystem.playSound(audioStore, soundId);
    }
}

glm::vec2 ProjectileMovementComponent::getRenderOffset(int delta) const
{
    // TODO: We never actually move the projectile, we just simulate the movement with a render offset.
    // This may cause problems later down the line with fog of war.
    // The original game seems to get around this by revealing a small area around the projectile,
    // provided the target is visible.

    // Calculate the start and end position, in pixels.
    // This doesn't have to be precise, as long as the distances are correct; we only need the render offset relative to
    // the start position.
    const MapNode pos = entity->getPos();
    const glm::vec2 start = { //
        static_cast<float>(RenderUtils::tileToPx_X(pos.x)),
        static_cast<float>(RenderUtils::tileToPx_Y(pos.x, pos.y))
    };
    const glm::vec2 end = { //
        static_cast<float>(RenderUtils::tileToPx_X(target.x)),
        static_cast<float>(RenderUtils::tileToPx_Y(target.x, target.y))
    };

    // Calculate the final render offset
    const glm::vec2 startToEnd = end - start;
    const glm::vec2 finalOffset = startToEnd * visualDistanceMultiplier;

    // Calculate current progress through the projectile's journey
    const float timeElapsedWithDelta = static_cast<float>(timeElapsed + delta);
    const float progress = MathUtils::clampf(timeElapsedWithDelta / lifetime, 0.f, 1.f);

    return finalOffset * progress;
}

}  // namespace Rival
