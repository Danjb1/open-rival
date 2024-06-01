#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "entity/Entity.h"
#include "entity/components/AttackComponent.h"
#include "entity/components/MovementListener.h"
#include "game/UnitType.h"

namespace Rival {

class MovementComponent;
struct MapNode;

enum class UnitState : std::uint8_t
{
    Idle,
    Moving,
    Attacking
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
{
public:
    static constexpr EntityType staticEntityType = EntityType::Unit;
    static constexpr int width = 1;
    static constexpr int height = 1;

public:
    Unit(UnitType type, std::string name, bool isNameUnique)
        : Entity(staticEntityType, width, height)
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

    void addStateListener(UnitStateListener* listener);
    void removeStateListener(UnitStateListener* listener);

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

    /** Sets this Unit's state. */
    void setState(UnitState state);

    /** Determines if the Unit is currently moving or performing an action. */
    bool isBusy() const;

    /** Aborts the action currently in progress. */
    void abortAction();

private:
    /** Gets a weak pointer to this Unit.
     * NOTE: This requires that this Unit was created using std::make_shared. */
    std::weak_ptr<Unit> getWeakThis();

private:
    std::string name;
    bool isNameUnique;

    std::unordered_set<UnitStateListener*> stateListeners;

    std::weak_ptr<MovementComponent> weakMovementComponent;
    std::weak_ptr<AttackComponent> weakAttackComponent;

    UnitType type = UnitType::Invalid;

    UnitState state = UnitState::Idle;
};

}  // namespace Rival
