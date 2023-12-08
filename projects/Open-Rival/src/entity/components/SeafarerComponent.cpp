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

void SeafarerPassability::onUnitPreparingMove(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Water | TilePassability::GroundUnitPendingMove);
}

void SeafarerPassability::onUnitLeavingTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Water | TilePassability::GroundUnitLeaving);
}

void SeafarerPassability::onUnitEnteringTile(WritablePathfindingMap& map, const MapNode& node, bool isPassingThrough)
{
    const TilePassability newPassability =
            isPassingThrough ? TilePassability::FlyingUnitLeaving : TilePassability::FlyingUnit;
    map.setPassability(node, TilePassability::Water | newPassability);
}

void SeafarerPassability::onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Water);
}

void SeafarerPassability::onUnitStopped(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Water | TilePassability::GroundUnit);
}

SeafarerComponent::SeafarerComponent()
    : MovementComponent(SeafarerComponent::seafarerPassability, SeafarerComponent::seafarerPassability)
{
}

}  // namespace Rival
