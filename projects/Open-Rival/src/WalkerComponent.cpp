#include "pch.h"

#include "WalkerComponent.h"

#include "MapUtils.h"
#include "Tile.h"
#include "World.h"

namespace Rival {

WalkerPassability WalkerComponent::walkerPassability = WalkerPassability();

bool WalkerPassability::isNodeTraversable(const PathfindingMap& map, const MapNode& node) const
{
    return map.getPassability(node) == TilePassability::Clear;
}

void WalkerPassability::onUnitLeftTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Clear);
}

void WalkerPassability::onUnitEnteredTile(WritablePathfindingMap& map, const MapNode& node)
{
    map.setPassability(node, TilePassability::Blocked);
}

WalkerComponent::WalkerComponent()
    : MovementComponent(WalkerComponent::walkerPassability, WalkerComponent::walkerPassability)
{
}

}  // namespace Rival
