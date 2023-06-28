#include "WalkerComponent.h"

#include "MapUtils.h"
#include "World.h"

namespace Rival {

WalkerPassability WalkerComponent::walkerPassability = WalkerPassability();

bool WalkerPassability::isNodePathable(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return (passability & unpathableFlags) == TilePassability::Clear;
}

bool WalkerPassability::isNodeTraversable(const PathfindingMap& map, const MapNode& node) const
{
    TilePassability passability = map.getPassability(node);
    return (passability & untraversableFlags) == TilePassability::Clear;
}

void WalkerPassability::onUnitLeavingTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::GroundUnitLeaving);
}

void WalkerPassability::onUnitEnteringTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::GroundUnit);
}

void WalkerPassability::onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Clear);
}

void WalkerPassability::onUnitEnteredTile(WritablePathfindingMap&, const MapNode&)
{
    // Nothing to do (passability has already been set by `onUnitEnteringTile`)
}

WalkerComponent::WalkerComponent()
    : MovementComponent(WalkerComponent::walkerPassability, WalkerComponent::walkerPassability)
{
}

}  // namespace Rival
