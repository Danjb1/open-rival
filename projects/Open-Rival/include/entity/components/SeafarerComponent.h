#pragma once

#include <string>

#include "entity/components/MovementComponent.h"
#include "game/Pathfinding.h"
#include "game/Tile.h"

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
    bool isNodeObstructed(const PathfindingMap& map, const MapNode& node) const override;
    bool isNodeTraversable(const PathfindingMap& map, const MapNode& node) const override;
    // End PassabilityChecker override

    // Begin PassabilityUpdater override
    void onUnitPreparingMove(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitLeavingTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitEnteringTile(WritablePathfindingMap& map, const MapNode& node, bool isPassingThrough) override;
    void onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitStopped(WritablePathfindingMap& map, const MapNode& node) override;
    // End PassabilityUpdater override

private:
    bool isWater(TilePassability passability) const;

private:
    /** TilePassability values that block pathfinding.
     * Using a value of "Clear" here is equivalent to "none"; seafaring units can pathfind anywhere on water
     * (this requirement is enforced elsewhere). */
    static constexpr TilePassability unpathableFlags = TilePassability::Clear;

    /** TilePassability values that signify an obstruction. */
    static constexpr TilePassability obstructedFlags = TilePassability::GroundUnit;

    /** TilePassability values that block traversal. */
    static constexpr TilePassability untraversableFlags =
            TilePassability::GroundUnit | TilePassability::GroundUnitPendingMove | TilePassability::GroundUnitLeaving;
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
