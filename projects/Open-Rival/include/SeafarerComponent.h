#pragma once

#include <string>

#include "MovementComponent.h"
#include "Pathfinding.h"
#include "Tile.h"

namespace Rival {

/**
 * PassabilityChecker that treats empty ground tiles as traversable.
 */
class SeafarerPassability
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
    bool isWater(TilePassability passability) const;

private:
    static constexpr TilePassability unpathableFlags = TilePassability::GroundUnit;

    // We have to wait for a unit to leave before we can move into their tile
    static constexpr TilePassability untraversableFlags = unpathableFlags | TilePassability::GroundUnitLeaving;
};

/**
 * Component that allows an entity to move on water.
 */
class SeafarerComponent : public MovementComponent
{
public:
    SeafarerComponent();

private:
    static SeafarerPassability seafarerPassability;
};

}  // namespace Rival
