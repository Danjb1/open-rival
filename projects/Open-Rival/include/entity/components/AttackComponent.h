#pragma once

#include <cstdint>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "entity/components/AnimationListener.h"
#include "entity/components/EntityComponent.h"
#include "entity/components/MovementListener.h"
#include "entity/components/UnitAnimationComponent.h"
#include "game/AttackDef.h"
#include "game/MapUtils.h"
#include "utils/CollectionUtils.h"

namespace Rival {

class AttackListener;
class AudioStore;
class AudioSystem;
class DataStore;
class Entity;
class EntityFactory;
class FacingComponent;
class MovementComponent;
class World;

enum class AttackState : std::uint8_t
{
    None,
    WaitingForIdle,
    Attacking,
    Cooldown
};

/**
 * Component that allows an entity to attack.
 */
class AttackComponent
    : public EntityComponent
    , public AnimationListener<UnitAnimationType>
{
public:
    AttackComponent(const AudioStore& audioStore,
            const DataStore& dataStore,
            AudioSystem& audioSystem,
            std::shared_ptr<const EntityFactory> entityFactory,
            std::shared_ptr<std::mt19937> randomizer);

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World*) override;
    void destroy() override;
    void update(int delta) override;
    // End EntityComponent override

    // Begin AnimationListener override
    void onAnimationFinished(UnitAnimationType animType) override;
    // End AnimationListener override

    void clearTarget();
    void setTargetEntity(std::weak_ptr<Entity> newTarget);
    void setTargetTile(const MapNode& newTargetTile);

    void addListener(std::weak_ptr<AttackListener> listener);
    void removeListener(std::weak_ptr<AttackListener> listener);

    void registerAttack(const AttackDef* attackDefinition);

private:
    bool hasValidTarget() const;
    bool isValidTarget(std::shared_ptr<Entity> targetEntity) const;

    void tryAttackTargetEntity(std::shared_ptr<Entity> targetEntity);
    void tryAttackTargetTile();

    void spawnProjectile(const AttackDef& attack, const MapNode& targetPos);

    void switchToNewTarget();
    void updateCooldown(int delta);

    bool isTargetInRange() const;
    bool isInRange(const std::shared_ptr<Entity> target) const;
    bool isTargetTileInRange() const;
    MapNode getTargetPos() const;

    void requestAttack();
    void startAttack();
    void deliverAttack();

    void tryMoveToTarget();
    void moveToTarget(const MapNode& node);

public:
    static const std::string key;

private:
    /** Number of ticks to wait before trying to repath to an unreachable target. */
    static constexpr int repathDelay = 60;

    /** Number of attempts to plan a route to the target before giving up. */
    static constexpr int maxMovementAttempts = 10;

    const AudioStore& audioStore;
    const DataStore& dataStore;
    AudioSystem& audioSystem;
    std::shared_ptr<const EntityFactory> entityFactory;
    std::shared_ptr<std::mt19937> randomizer;

    WeakPtrSet<AttackListener> listeners;

    /** Target of the current attack. */
    std::weak_ptr<Entity> weakTargetEntity;
    /** Requested target (next attack). */
    std::weak_ptr<Entity> weakRequestedTargetEntity;

    /** Target tile, in case an attack is directed at thin air.
     * Only ranged units can ever have a target tile.
     * (Melee units trying to attack thin air should get the message "Could not attack there!".)
     * If another unit walks into this tile, they should still take projectile damage! */
    MapNode targetTile = MapNode::Invalid;

    std::weak_ptr<MovementComponent> weakMovementComp;
    std::weak_ptr<FacingComponent> weakFacingComp;
    std::weak_ptr<UnitAnimationComponent> weakAnimationComp;

    std::vector<const AttackDef*> attackDefinitions;

    AttackState attackState = AttackState::None;

    float cooldownTimeElapsed = 0;
    int cooldownDuration = 0;

    MapNode lastTargetPosition;
    int moveToTargetCooldown = 0;
    int numMovementAttempts = 0;
};

}  // namespace Rival
