#pragma once

#include "MovementComponent.h"
#include "Pathfinding.h"

namespace Rival {

/**
 * PassabilityChecker that treats empty ground tiles as traversable.
 */
class WalkerPassabilityChecker : public Pathfinding::PassabilityChecker
{
    bool isNodeTraversable(const PathfindingMap& map, const MapNode& node) const override;
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
    static WalkerPassabilityChecker walkerPassabilityChecker;
};

}  // namespace Rival
