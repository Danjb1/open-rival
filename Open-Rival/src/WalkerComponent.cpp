#include "pch.h"
#include "WalkerComponent.h"

#include "MapUtils.h"

namespace Rival {

    const std::string WalkerComponent::key = "walker";

    WalkerPassabilityChecker WalkerComponent::passabilityChecker =
            WalkerPassabilityChecker();

    WalkerComponent::WalkerComponent() : EntityComponent(key) {}

    bool WalkerPassabilityChecker::isNodeTraversable(
            const PathfindingMap& map, const MapNode& node) const {
        return map.getPassability(node.x, node.y) == TilePassability::Clear;
    }

    void WalkerComponent::update() {
        // TMP: plan a route
        if (entity->getId() == 1 && route.isEmpty()) {
            route = Pathfinding::findPath(
                    { entity->getX(), entity->getY() },
                    { 4, 3 },
                    *entity->getScenario(),
                    passabilityChecker);
        }

        // TMP: wait between movements
        if (delay < 10) {
            ++delay;
            return;
        } else {
            delay = 0;
        }

        if (canWalk()) {
            walkToNextNode();
        }
    }

    void WalkerComponent::setRoute(Pathfinding::Route newRoute) {
        route = newRoute;
    }

    bool WalkerComponent::canWalk() {
        return !route.isEmpty();
    }

    void WalkerComponent::walkToNextNode() {
        MapNode node = route.pop();
        entity->setPos(node.x, node.y);
    }

}  // namespace Rival
