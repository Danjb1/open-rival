#pragma once

#include <memory>
#include <string>

#include "entity/components/EntityComponent.h"
#include "entity/components/MovementListener.h"
#include "game/Pathfinding.h"
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
class MovementComponent : public EntityComponent
{
public:
    MovementComponent(const Pathfinding::PassabilityChecker& passabilityChecker,
            Pathfinding::PassabilityUpdater& passabilityUpdater);
    virtual ~MovementComponent() = default;

    // Begin EntityComponent override
    void update() override;
    // End EntityComponent override

    void addListener(std::weak_ptr<MovementListener> listener);
    void removeListener(std::weak_ptr<MovementListener> listener);

    /** Called before a group movement in order to help prevent units obstructing each other. */
    void prepareForMovement();

    /** Starts moving to the given node, if a route can be found. */
    void moveTo(const MapNode& node, Pathfinding::Context& context, Pathfinding::Hints hints = {});

    /**
     * Gets the movement that's currently in progress.
     */
    const Movement& getMovement() const
    {
        return movement;
    }

    const Pathfinding::Route& getRoute() const
    {
        return route;
    }

private:
    MapNode getStartPosForNextMovement() const;
    void updateMovement();
    bool tryStartNextMovement();
    void startNextMovement(PathfindingMap& map);
    void resetPassability();

    bool tryRepathAroundObstruction(const PathfindingMap& map, Pathfinding::Hints hints = {});
    bool tryRepathAroundTemporaryObstruction(const PathfindingMap& map);
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

    // TMP: This should depend on the unit's speed
    int ticksPerMove = 30;

private:
    static constexpr float horizontalMoveTimeMultiplier = 1.5f;
    static constexpr int maxTicksToWaitForTileToClear = 4;

    int ticksSpentWaiting = 0;
};

}  // namespace Rival
