#pragma once

#include <string>

#include "MovementComponent.h"
#include "Pathfinding.h"

namespace Rival {

/**
 * PassabilityChecker that treats empty ground tiles as traversable.
 */
class WalkerPassability
    : public Pathfinding::PassabilityChecker
    , public Pathfinding::PassabilityUpdater
{
    // Begin PassabilityChecker override
    bool isNodeTraversable(const PathfindingMap& map, const MapNode& node) const override;
    // End PassabilityChecker override

    // Begin PassabilityUpdater override
    void onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitEnteredTile(WritablePathfindingMap& map, const MapNode& node) override;
    // End PassabilityUpdater override
};

/**
 * Component that allows an entity to walk around the map.
 *
 * Note that during movement, entities are considered to occupy their
 * original tile until they have fully moved into the new tile.
 */
class WalkerComponent : public MovementComponent
{
public:
    WalkerComponent();

private:
    static WalkerPassability walkerPassability;
};

}  // namespace Rival
