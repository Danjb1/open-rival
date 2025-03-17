#pragma once

#include <memory>
#include <string>

#include "entity/Effect.h"
#include "entity/Entity.h"
#include "entity/components/AnimationListener.h"
#include "entity/components/AttackListener.h"
#include "entity/components/FacingComponent.h"
#include "entity/components/HealthComponent.h"
#include "entity/components/MovementListener.h"
#include "game/UnitType.h"
#include "utils/CollectionUtils.h"

namespace Rival {

class AttackComponent;
class DeathEffectComponent;
class FacingComponent;
class MovementComponent;
class SpriteComponent;
class UnitAnimationComponent;
struct MapNode;

enum class UnitState : std::uint8_t
{
    Idle,
    WaitingToMove,
    Moving,
    Attacking,
    Dying
};

/** Interface used to listen to unit state changes. */
class UnitStateListener
{
public:
    virtual void onUnitStateChanged(const UnitState newState) = 0;
};

/** A Unit that can be added to the world. */
class Unit
    : public Entity
    , public MovementListener
    , public AttackListener
    , public HealthListener
    , public AnimationListener<UnitAnimationType>
    , public EffectListener
{
public:
    static constexpr EntityType staticEntityType = EntityType::Unit;

public:
    Unit(UnitType type, std::string name, bool isNameUnique)
        : Entity(staticEntityType, /* isBig = */ false)
        , type(type)
        , name(name)
        , isNameUnique(isNameUnique)
    {
    }

    // Begin Entity override
    void onReady() override;
    void onDestroy() override;
    // End Entity override

    // Begin MovementListener override
    void onUnitMoveStart(const MapNode* nextNode) override;
    void onUnitPaused() override;
    void onUnitStopped() override;
    // End MovementListener override

    // Begin AttackListener override
    void onAttackStarted() override;
    void onAttackFinished() override;
    // End AttackListener override

    // Begin HealthListener override
    void onHealthChanged(int prevValue, int newValue) override;
    void onMaxHealthChanged(int prevValue, int newValue) override;
    void onHealthDepleted() override;
    // Begin HealthListener override

    // Begin AnimationListener override
    void onAnimationFinished(UnitAnimationType animType) override;
    // End AnimationListener override

    // Begin EffectListener override
    void onEffectFinished(Effect* effect) override;
    // End EffectListener override

    void addStateListener(std::weak_ptr<UnitStateListener> listener);
    void removeStateListener(std::weak_ptr<UnitStateListener> listener);

    UnitType getUnitType() const
    {
        return type;
    }

    const std::string& getName() const
    {
        return name;
    }

    /** Gets the Unit's current state. */
    UnitState getState() const
    {
        return state;
    }

    /** Sets this Unit's state.
     * Normally, a Unit's state should not be changed if the Unit is busy.
     * Call `abortAction` instead and wait for the Unit to become free. */
    void setState(UnitState state);

    /** Determines if the Unit is currently busy.
     * Call `abortAction` to free up a busy Unit. */
    bool isBusy() const;

    /** Aborts the action currently in progress.
     * It may take some time for the action to be fully stopped. */
    void abortAction();

private:
    /** Gets a weak pointer to this Unit.
     * NOTE: This requires that this Unit was created using std::make_shared. */
    std::weak_ptr<Unit> getWeakThis();

    void trySpawnDeathEffect();
    void triggerDeathEvent();

private:
    std::string name;
    bool isNameUnique;

    WeakPtrSet<UnitStateListener> stateListeners;

    std::weak_ptr<MovementComponent> weakMovementComponent;
    std::weak_ptr<AttackComponent> weakAttackComponent;
    std::weak_ptr<HealthComponent> weakHealthComponent;
    std::weak_ptr<FacingComponent> weakFacingComponent;
    std::weak_ptr<SpriteComponent> weakSpriteComponent;
    std::weak_ptr<UnitAnimationComponent> weakAnimationComponent;
    std::weak_ptr<DeathEffectComponent> weakDeathEffectComponent;

    UnitType type = UnitType::Invalid;

    UnitState state = UnitState::Idle;
};

}  // namespace Rival
