#include "pch.h"
#include "WalkerComponent.h"

#include "Entity.h"
#include "MapUtils.h"
#include "Scenario.h"
#include "TimerUtils.h"

namespace Rival {

    WalkerPassabilityChecker WalkerComponent::passabilityChecker =
            WalkerPassabilityChecker();

    WalkerComponent::WalkerComponent() : movement({ 0, 0 }) {}

    bool WalkerPassabilityChecker::isNodeTraversable(
            const PathfindingMap& map, const MapNode& node) const {
        return map.getPassability(node) == TilePassability::Clear;
    }

    void WalkerComponent::update() {
        // TMP: plan a route
        if (entity->getId() == 1 && route.isEmpty()) {
            route = Pathfinding::findPath(
                    entity->getPos(),
                    { 4, 3 },
                    *entity->getScenario(),
                    passabilityChecker);

            if (!route.isEmpty()) {
                prepareNextMovement();
            }
        }

        if (route.isEmpty()) {
            return;
        }

        entity->moved = true;

        // Update movement
        if (movement.timeElapsed >= movement.timeRequired) {
            completeMovement();
        } else {
            movement.timeElapsed += TimerUtils::timeStepMs;
        }
    }

    void WalkerComponent::setRoute(Pathfinding::Route newRoute) {
        route = newRoute;
    }

    /**
     * Called before moving to a new tile.
     */
    void WalkerComponent::prepareNextMovement() {
        if (route.isEmpty()) {
            return;
        }

        movement.timeRequired = ticksPerMove * TimerUtils::timeStepMs;

        for (MovementListener* listener : listeners) {
            listener->onUnitMoveStart(route.peek());
        }
    }

    /**
     * Called after moving to a new tile.
     */
    void WalkerComponent::completeMovement() {
        MapNode node = route.pop();
        entity->setPos(node);

        movement.timeElapsed = 0;

        if (route.isEmpty()) {
            for (MovementListener* listener : listeners) {
                listener->onUnitJourneyEnd();
            }
        } else {
            prepareNextMovement();
        }
    }

}  // namespace Rival
