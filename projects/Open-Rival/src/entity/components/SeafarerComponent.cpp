#include "entity/components/SeafarerComponent.h"

#include "game/MapUtils.h"
#include "game/World.h"

namespace Rival {

SeafarerPassability SeafarerComponent::seafarerPassability = SeafarerPassability();

bool SeafarerPassability::isNodePathable(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return isWater(passability) && (passability & unpathableFlags) == TilePassability::Clear;
}

bool SeafarerPassability::isNodeObstructed(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return (passability & obstructedFlags) != TilePassability::Clear;
}

bool SeafarerPassability::isNodeTraversable(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return isWater(passability) && (passability & untraversableFlags) == TilePassability::Clear;
}

bool SeafarerPassability::isWater(TilePassability passability) const
{
    return (passability & TilePassability::Water) != TilePassability::Clear;
}

void SeafarerPassability::onUnitPreparingMove(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node, TilePassability::GroundUnit);
    map.addPassability(node, TilePassability::GroundUnitPendingMove);
}

void SeafarerPassability::onUnitLeavingTile(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node, TilePassability::GroundUnitPendingMove);
    map.addPassability(node, TilePassability::GroundUnitLeaving);
}

void SeafarerPassability::onUnitEnteringTile(PathfindingMap& map, const MapNode& node, bool isPassingThrough)
{
    map.addPassability(node, isPassingThrough ? TilePassability::GroundUnitLeaving : TilePassability::GroundUnit);
}

void SeafarerPassability::onUnitLeftTile(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node, TilePassability::GroundUnitLeaving);
}

void SeafarerPassability::onUnitStopped(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node, TilePassability::GroundUnitPendingMove | TilePassability::GroundUnitLeaving);
    map.addPassability(node, TilePassability::GroundUnit);
}

void SeafarerPassability::onUnitMoveAborted(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node,
            TilePassability::GroundUnit | TilePassability::GroundUnitPendingMove | TilePassability::GroundUnitLeaving);
}

SeafarerComponent::SeafarerComponent()
    : MovementComponent(
              MovementMode::Seafaring, SeafarerComponent::seafarerPassability, SeafarerComponent::seafarerPassability)
{
}

}  // namespace Rival
