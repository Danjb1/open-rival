#pragma once

#include <string>

#include "MovementComponent.h"
#include "Pathfinding.h"
#include "Tile.h"

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
    bool isNodeTraversable(const PathfindingMap& map, const MapNode& node) const override;
    // End PassabilityChecker override

    // Begin PassabilityUpdater override
    void onUnitLeavingTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitEnteringTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node) override;
    void onUnitEnteredTile(WritablePathfindingMap& map, const MapNode& node) override;
    // End PassabilityUpdater override

private:
    static constexpr TilePassability unpathableFlags =  //
            TilePassability::Blocked                    //
            | TilePassability::Building                 //
            | TilePassability::Coastline                //
            | TilePassability::GroundUnit               //
            | TilePassability::SoftMountain             //
            | TilePassability::Tree                     //
            | TilePassability::Water;

    // We have to wait for a unit to leave before we can move into their tile
    static constexpr TilePassability untraversableFlags = unpathableFlags | TilePassability::GroundUnitLeaving;
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
