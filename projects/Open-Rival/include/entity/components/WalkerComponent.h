#pragma once

#include <string>

#include "entity/components/MovementComponent.h"
#include "game/Pathfinding.h"
#include "game/Tile.h"

namespace Rival {

/**
 * PassabilityChecker that treats empty ground tiles as traversable.
 */
class WalkerPassability
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
    static constexpr TilePassability unpathableFlags =  //
            TilePassability::Blocked                    //
            | TilePassability::Building                 //
            | TilePassability::Coastline                //
            | TilePassability::SoftMountain             //
            | TilePassability::Tree                     //
            | TilePassability::Water                    //
            | TilePassability::Impassable;

    /** TilePassability values that signify an obstruction. */
    static constexpr TilePassability obstructedFlags = TilePassability::GroundUnit;

    /** TilePassability values that block traversal. */
    static constexpr TilePassability untraversableFlags = unpathableFlags  //
            | TilePassability::GroundUnit                                  //
            | TilePassability::GroundUnitPendingMove                       //
            | TilePassability::GroundUnitLeaving;
};

/**
 * Component that allows an entity to walk around the map.
 */
class WalkerComponent : public MovementComponent
{
public:
    WalkerComponent();

private:
    static WalkerPassability walkerPassability;
};

}  // namespace Rival
