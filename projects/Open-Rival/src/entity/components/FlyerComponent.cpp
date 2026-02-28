#include "entity/components/FlyerComponent.h"

#include "game/MapUtils.h"
#include "game/World.h"

namespace Rival {

FlyerPassability FlyerComponent::flyerPassability = FlyerPassability();

bool FlyerPassability::isNodePathable(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return (passability & unpathableFlags) == TilePassability::Clear;
}

bool FlyerPassability::isNodeObstructed(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return (passability & obstructedFlags) != TilePassability::Clear;
}

bool FlyerPassability::isNodeTraversable(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return (passability & untraversableFlags) == TilePassability::Clear;
}

void FlyerPassability::onUnitPreparingMove(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node, TilePassability::FlyingUnit);
    map.addPassability(node, TilePassability::FlyingUnitPendingMove);
}

void FlyerPassability::onUnitLeavingTile(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node, TilePassability::FlyingUnitPendingMove);
    map.addPassability(node, TilePassability::FlyingUnitLeaving);
}

void FlyerPassability::onUnitEnteringTile(PathfindingMap& map, const MapNode& node, bool isPassingThrough)
{
    map.addPassability(node, isPassingThrough ? TilePassability::FlyingUnitLeaving : TilePassability::FlyingUnit);
}

void FlyerPassability::onUnitLeftTile(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node, TilePassability::FlyingUnitLeaving);
}

void FlyerPassability::onUnitStopped(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node, TilePassability::FlyingUnitPendingMove | TilePassability::FlyingUnitLeaving);
    map.addPassability(node, TilePassability::FlyingUnit);
}

void FlyerPassability::onUnitMoveAborted(PathfindingMap& map, const MapNode& node)
{
    map.removePassability(node,
            TilePassability::FlyingUnit | TilePassability::FlyingUnitPendingMove | TilePassability::FlyingUnitLeaving);
}

FlyerComponent::FlyerComponent()
    : MovementComponent(MovementMode::Flying, FlyerComponent::flyerPassability, FlyerComponent::flyerPassability)
{
}

}  // namespace Rival
