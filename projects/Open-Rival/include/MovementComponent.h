#pragma once

#include <unordered_set>

#include "EntityComponent.h"
#include "Pathfinding.h"

namespace Rival {

struct MapNode;

/**
 * Represents a movement between 2 tiles.
 */
struct Movement
{
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
    bool isFinished() const;
};

/**
 * Interface used to listen to movements.
 */
class MovementListener
{
public:
    virtual void onUnitMoveStart(const MapNode* nextNode) = 0;
    virtual void onUnitStopped() = 0;
};

/**
 * Base class for a component that allows entities to move between tiles.
 */
class MovementComponent : public EntityComponent
{
public:
    MovementComponent(Pathfinding::PassabilityChecker* passabilityChecker);
    virtual ~MovementComponent() = default;

    // Begin EntityComponent override
    void update() override;
    // End EntityComponent override

    void addListener(MovementListener* listener);
    void removeListener(MovementListener* listener);

    void moveTo(MapNode node);

    /**
     * Gets the movement that's currently in progress.
     */
    const Movement& getMovement() const
    {
        return movement;
    }

private:
    MapNode getStartPosForNextMovement() const;
    void setRoute(Pathfinding::Route route);
    void updateMovement();
    void prepareNextMovement();
    void completeMovement();

public:
    static const std::string key;

protected:
    Pathfinding::PassabilityChecker* passabilityChecker;

    std::unordered_set<MovementListener*> listeners;

    Pathfinding::Route route;

    Movement movement;

    // TMP: This should depend on the unit's speed
    int ticksPerMove = 30;
};

}  // namespace Rival
