#pragma once

#include "MovementComponent.h"
#include "Pathfinding.h"
#include "Tile.h"

namespace Rival {

/**
 * PassabilityChecker that treats all tiles as traversable unless occupied by another flyer.
 */
class FlyerPassability
    : public Pathfinding::PassabilityChecker
    , public Pathfinding::PassabilityUpdater
{
public:
    // Begin PassabilityChecker override
    bool isNodePathable(const PathfindingMap& map, const MapNode& node) const override;
    bool isNodeTraversable(const PathfindingMap& map, const MapNode& node) const override;
    // End PassabilityChecker override

    // Begin PassabilityUpdater override
    void onUnitLeavingTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitEnteringTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitEnteredTile(WritablePathfindingMap& map, const MapNode& node) override;
    // End PassabilityUpdater override

private:
    static constexpr TilePassability unpathableFlags = TilePassability::FlyingUnit;

    // We have to wait for a unit to leave before we can move into their tile
    static constexpr TilePassability untraversableFlags = unpathableFlags | TilePassability::FlyingUnitLeaving;
};

/**
 * Component that allows an entity to fly around the map.
 */
class FlyerComponent : public MovementComponent
{
public:
    FlyerComponent();

private:
    static FlyerPassability flyerPassability;
};

}  // namespace Rival
