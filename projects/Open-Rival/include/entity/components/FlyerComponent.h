#pragma once

#include "entity/components/MovementComponent.h"
#include "game/Pathfinding.h"
#include "game/Tile.h"

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
    bool isNodeObstructed(const PathfindingMap& map, const MapNode& node) const override;
    bool isNodeTraversable(const PathfindingMap& map, const MapNode& node) const override;
    // End PassabilityChecker override

    // Begin PassabilityUpdater override
    void onUnitPreparingMove(PathfindingMap& map, const MapNode& node) override;
    void onUnitLeavingTile(PathfindingMap& map, const MapNode& node) override;
    void onUnitEnteringTile(PathfindingMap& map, const MapNode& node, bool isPassingThrough) override;
    void onUnitLeftTile(PathfindingMap& map, const MapNode& node) override;
    void onUnitStopped(PathfindingMap& map, const MapNode& node) override;
    void onUnitMoveAborted(PathfindingMap& map, const MapNode& node) override;
    // End PassabilityUpdater override

private:
    /** TilePassability values that block pathfinding. */
    static constexpr TilePassability unpathableFlags = TilePassability::Impassable;

    /** TilePassability values that signify an obstruction. */
    static constexpr TilePassability obstructedFlags = TilePassability::FlyingUnit;

    /** TilePassability values that block traversal. */
    static constexpr TilePassability untraversableFlags = unpathableFlags  //
            | TilePassability::FlyingUnit                                  //
            | TilePassability::FlyingUnitPendingMove                       //
            | TilePassability::FlyingUnitLeaving;
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
