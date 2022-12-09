#include "pch.h"

#include "FlyerComponent.h"

#include "MapUtils.h"
#include "World.h"

namespace Rival {

FlyerPassability FlyerComponent::flyerPassability = FlyerPassability();

bool FlyerPassability::isNodePathable(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return (passability & unpathableFlags) == TilePassability::Clear;
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

void FlyerPassability::onUnitEnteringTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::FlyingUnit);
}

void FlyerPassability::onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Clear);
}

void FlyerPassability::onUnitEnteredTile(WritablePathfindingMap&, const MapNode&)
{
    // Nothing to do (passability has already been set by `onUnitEnteringTile`)
}

FlyerComponent::FlyerComponent()
    : MovementComponent(FlyerComponent::flyerPassability, FlyerComponent::flyerPassability)
{
}

}  // namespace Rival
