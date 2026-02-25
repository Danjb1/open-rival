#pragma once

#include <string>

#include "entity/components/MovementComponent.h"
#include "game/Pathfinding.h"
#include "game/Tile.h"

namespace Rival {

/**
 * PassabilityChecker that treats empty water tiles as traversable.
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
    void onUnitPreparingMove(PathfindingMap& map, const MapNode& node) override;
    void onUnitLeavingTile(PathfindingMap& map, const MapNode& node) override;
    void onUnitEnteringTile(PathfindingMap& map, const MapNode& node, bool isPassingThrough) override;
    void onUnitLeftTile(PathfindingMap& map, const MapNode& node) override;
    void onUnitStopped(PathfindingMap& map, const MapNode& node) override;
    void onUnitMoveAborted(PathfindingMap& map, const MapNode& node) override;
    // End PassabilityUpdater override

private:
    bool isWater(TilePassability passability) const;

private:
    /** TilePassability values that block pathfinding.
     * (The requirement that tiles are water is enforced elsewhere.) */
    static constexpr TilePassability unpathableFlags = TilePassability::Impassable;

    /** TilePassability values that signify an obstruction. */
    static constexpr TilePassability obstructedFlags = TilePassability::GroundUnit;

    /** TilePassability values that block traversal. */
    static constexpr TilePassability untraversableFlags = unpathableFlags  //
            | TilePassability::GroundUnit                                  //
            | TilePassability::GroundUnitPendingMove                       //
            | TilePassability::GroundUnitLeaving;
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
