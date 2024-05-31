#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "entity/components/EntityComponent.h"
#include "entity/components/MovementListener.h"
#include "game/MapUtils.h"
#include "utils/CollectionUtils.h"

namespace Rival {

class Entity;
class FacingComponent;
class MovementComponent;
class World;

enum class AttackState : std::uint8_t
{
    None,
    WaitingForMovement,
    Attacking,
    Cooldown
};

/**
 * Interface used to listen to attacks.
 */
class AttackListener
{
public:
    /** Called when an entity starts attacking. */
    virtual void onAttackStarted() = 0;

    /** Called when an entity's attack is "launched",
     * e.g. a melee hit is attempted or a projectile is spawned. */
    virtual void onAttackLaunched() = 0;
};

/**
 * Component that allows an entity to attack.
 */
class AttackComponent
    : public EntityComponent
    , public MovementListener
{
public:
    AttackComponent();

    // Begin EntityComponent override
    void onEntityAddedToWorld(World*) override;
    void destroy() override;
    void update(int delta) override;
    // End EntityComponent override

    // Begin MovementComponent override
    void onUnitMoveStart(const MapNode* nextNode) override;
    void onUnitPaused() override;
    void onUnitStopped() override;
    // End MovementComponent override

    void setTarget(std::weak_ptr<Entity> newTarget);

    void addListener(std::weak_ptr<AttackListener> listener);
    void removeListener(std::weak_ptr<AttackListener> listener);

private:
    bool isInRange(const MapNode& node) const;
    void requestAttack(std::shared_ptr<Entity> targetEntity);
    void startAttack(std::shared_ptr<Entity> targetEntity);
    void moveToTarget(const MapNode& node);

public:
    static const std::string key;

private:
    WeakPtrSet<AttackListener> listeners;
    std::weak_ptr<Entity> weakTargetEntity;
    std::weak_ptr<MovementComponent> weakMovementComp;
    std::weak_ptr<FacingComponent> weakFacingComp;

    AttackState attackState = AttackState::None;
};

}  // namespace Rival
