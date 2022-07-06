#pragma once

#include <cstdint>  // uint8_t
#include <memory>
#include <string>
#include <unordered_set>

#include "EntityComponent.h"
#include "MovementComponent.h"
#include "Unit.h"

namespace Rival {

struct MapNode;

enum class UnitState : std::uint8_t
{
    Idle,
    Moving,
    Attacking
};

/**
 * Interface used to listen to unit state changes.
 */
class UnitStateListener
{
public:
    virtual void onUnitStateChanged(const UnitState newState) = 0;
};

/**
 * Component containing basic unit properties.
 */
class UnitPropsComponent
    : public EntityComponent
    , public MovementListener
{

public:
    UnitPropsComponent(Unit::Type type);

    // Begin EntityComponent override
    virtual void onEntitySpawned(World* scenario) override;
    virtual void onDelete() override;
    // End EntityComponent override

    // Begin MovementListener override
    virtual void onUnitMoveStart(const MapNode* nextNode) override;
    virtual void onUnitStopped() override;
    // End MovementListener override

    void addStateListener(UnitStateListener* listener);
    void removeStateListener(UnitStateListener* listener);

    Unit::Type getUnitType() const
    {
        return type;
    }

    /**
     * Gets the unit's current state.
     */
    UnitState getState() const
    {
        return state;
    }

    /**
     * Sets this Entity's state.
     */
    void setState(UnitState state);

public:
    static const std::string key;

private:
    std::unordered_set<UnitStateListener*> stateListeners;

    std::weak_ptr<MovementComponent> weakMovementComponent;

    Unit::Type type = Unit::Type::Invalid;

    UnitState state = UnitState::Idle;
};

}  // namespace Rival
