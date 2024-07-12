#pragma once

#include <memory>
#include <string>

#include "entity/Unit.h"
#include "entity/components/EntityComponent.h"
#include "entity/components/MovementListener.h"
#include "game/Pathfinding.h"
#include "game/UnitDef.h"
#include "utils/CollectionUtils.h"

namespace Rival {

class World;

/**
 * Represents a movement between 2 tiles.
 */
struct Movement
{
    MapNode start = { -1, -1 };
    MapNode destination = { -1, -1 };

    /**
     * Time spent moving so far, in ms.
     */
    int timeElapsed = 0;

    /**
     * Total time required to complete the movement, in ms.
     */
    int timeRequired = 0;

    void clear();
    bool isValid() const;
    bool isInProgress() const;
    bool isPastHalfway() const;
    bool isFinished() const;
};

/**
 * Base class for a component that allows entities to move between tiles.
 */
class MovementComponent
    : public EntityComponent
    , public UnitStateListener
{
public:
    MovementComponent(MovementMode movementMode,
            const Pathfinding::PassabilityChecker& passabilityChecker,
            Pathfinding::PassabilityUpdater& passabilityUpdater);

    virtual ~MovementComponent() = default;

    // Begin EntityComponent override
    void onEntityFirstAddedToWorld(World* world) override;
    void onEntityRemovedFromWorld(World* world) override;
    void destroy() override;
    void update(int delta) override;
    // End EntityComponent override

    // Begin UnitStateListener override
    void onUnitStateChanged(const UnitState newState) override;
    // End UnitStateListener override

    void addListener(std::weak_ptr<MovementListener> listener);
    void removeListener(std::weak_ptr<MovementListener> listener);

    /** Called before a group movement in order to help prevent units obstructing each other. */
    void prepareForMovement();

    /** Starts moving to the given node, if a route can be found. */
    void moveTo(const MapNode& node, Pathfinding::Context& context, Pathfinding::Hints hints = {});

    /** Cancels the current route (if present) as soon as we reach the next tile. */
    void requestStop();

    /** Determines if the unit is currently between tiles. */
    bool isCurrentlyMoving() const;

    /** Gets the movement that's currently in progress. */
    const Movement& getMovement() const
    {
        return movement;
    }

    const Pathfinding::Route& getRoute() const
    {
        return route;
    }

    MovementMode getMovementMode() const
    {
        return movementMode;
    }

private:
    MapNode getStartPosForNextMovement() const;
    void updateMovement(int delta);
    bool tryStartNextMovement();
    void startNextMovement(PathfindingMap& map);
    void resetPassability(bool wasRemoved);

    bool tryRepathAroundNextNode(const PathfindingMap& map);
    bool tryRepathAroundTemporaryObstruction(const PathfindingMap& map);
    bool tryRepathAroundObstruction(const PathfindingMap& map);
    bool tryRepath(Pathfinding::Hints hints = {});

    void onLeftPreviousTile();
    void onCompletedMoveToNewTile();
    void stopMovement();

public:
    static const std::string key;

protected:
    const Pathfinding::PassabilityChecker& passabilityChecker;
    Pathfinding::PassabilityUpdater& passabilityUpdater;

    WeakPtrSet<MovementListener> listeners;

    Pathfinding::Route route;

    Movement movement;

    MovementMode movementMode;

    // TMP: This should depend on the unit's speed
    int ticksPerMove = 30;

private:
    /** Move time multiplier when performing a horizontal (east/west) movement. */
    static constexpr float horizontalMoveTimeMultiplier = 1.5f;

    /** Maximum number of ticks to wait for a temporary obstruction to clear up before repathing.
     * Consider that it might take a slow-moving unit ~60 ticks to move out of a tile. */
    static constexpr int numTicksToWaitForTileToClear = 10;

    /** Maximum number of repathing attempts around a temporary obstruction before giving up.
     * This could result in units getting left behind when trying to move through a congested area, but we need some
     * kind of limit to prevent units pathfinding indefinitely when an obstruction is not clearing. */
    static constexpr int maxRepathAttempts = 20;

    Pathfinding::Hints cachedHints;

    int numFailedRepathAttempts = 0;
    int ticksSpentWaiting = 0;

    bool wasAbortActionRequested = false;
};

}  // namespace Rival
