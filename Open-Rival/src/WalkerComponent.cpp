#include "pch.h"
#include "WalkerComponent.h"

#include "Entity.h"
#include "MapUtils.h"
#include "Scenario.h"
#include "TimerUtils.h"

namespace Rival {

    const std::string WalkerComponent::key = "walker";

    WalkerPassabilityChecker WalkerComponent::passabilityChecker =
            WalkerPassabilityChecker();

    WalkerComponent::WalkerComponent() : EntityComponent(key),
                                         movement({ 0, 0 }) {}

    void WalkerComponent::onEntitySpawned(Scenario*) {
        unitPropsComponent = entity->getComponent<UnitPropsComponent>(
                FacingComponent::key);
        facingComponent = entity->getComponent<FacingComponent>(
                FacingComponent::key);
    }

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
            movement.timeRequired = ticksPerMove * TimerUtils::timeStepMs;
            unitPropsComponent->setState(UnitState::Moving);
        }

        if (!canWalk()) {
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

    bool WalkerComponent::canWalk() {
        return !route.isEmpty();
    }

    void WalkerComponent::completeMovement() {
        MapNode node = route.pop();
        entity->setPos(node);

        movement.timeElapsed = 0;

        if (route.isEmpty()) {
            unitPropsComponent->setState(UnitState::Idle);
            return;
        }

        if (facingComponent) {
            const MapNode* nextNode = route.peek();
            Facing newFacing = MapUtils::getDir(node, *nextNode);
            facingComponent->setFacing(newFacing);
        }
    }

}  // namespace Rival
