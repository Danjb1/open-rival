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
    return (passability & obstructedFlags) == TilePassability::Clear;
}

bool FlyerPassability::isNodeTraversable(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return (passability & untraversableFlags) == TilePassability::Clear;
}

void FlyerPassability::onUnitLeavingTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::FlyingUnitLeaving);
}

void FlyerPassability::onUnitEnteringTile(WritablePathfindingMap& map, const MapNode& node, bool isPassingThrough)
{
    map.setPassability(node, isPassingThrough ? TilePassability::FlyingUnitLeaving : TilePassability::FlyingUnit);
}

void FlyerPassability::onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Clear);
}

void FlyerPassability::onUnitStopped(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::FlyingUnit);
}

FlyerComponent::FlyerComponent()
    : MovementComponent(FlyerComponent::flyerPassability, FlyerComponent::flyerPassability)
{
}

}  // namespace Rival
