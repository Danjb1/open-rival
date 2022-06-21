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
    /**
     * Time spent moving so far, in ms.
     */
    int timeElapsed;

    /**
     * Total time required to complete the movement, in ms.
     */
    int timeRequired;
};

/**
 * Interface used to listen to movements.
 */
class MovementListener
{
public:
    virtual void onUnitMoveStart(const MapNode* nextNode) = 0;
    virtual void onUnitJourneyEnd() = 0;
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
     * Gets the current route.
     */
    const Pathfinding::Route getRoute() const
    {
        return route;
    }

    /**
     * Gets the movement that's currently in progress.
     */
    const Movement& getMovement() const
    {
        return movement;
    }

private:
    void setRoute(Pathfinding::Route route);
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
