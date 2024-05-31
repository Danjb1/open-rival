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

    /** Called when an entity finishes an attack. */
    virtual void onAttackFinished() = 0;
};

/** An attack that is currently being performed. */
struct AttackInstance
{
    /** Time spent in this attack so far. */
    int timeElapsed = 0;

    /** Total duration of the attack. */
    int duration = 0;

    /** Determines whether this attack is fully completed. */
    bool isFinished() const;

    void reset();
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
    void updateAttack(int delta);
    void deliverAttack();
    void updateCooldown(int delta);
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
    AttackInstance attackInstance;

    float cooldownTimeElapsed = 0;
    float cooldownDuration = 200;  // TODO: Depends on unit's attack speed?
};

}  // namespace Rival
