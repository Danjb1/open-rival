#include "pch.h"

#include "WalkerComponent.h"

#include "MapUtils.h"
#include "Tile.h"
#include "World.h"

namespace Rival {

WalkerPassabilityChecker WalkerComponent::walkerPassabilityChecker = WalkerPassabilityChecker();

bool WalkerPassabilityChecker::isNodeTraversable(const PathfindingMap& map, const MapNode& node) const
{
    return map.getPassability(node) == TilePassability::Clear;
}

WalkerComponent::WalkerComponent()
    : MovementComponent(&WalkerComponent::walkerPassabilityChecker)
{
}

}  // namespace Rival
