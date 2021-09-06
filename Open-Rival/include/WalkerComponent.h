#ifndef WALKER_COMPONENT_H
#define WALKER_COMPONENT_H

#include <vector>

#include "EntityComponent.h"
#include "FacingComponent.h"
#include "Pathfinding.h"

namespace Rival {

    /**
     * PassabilityChecker that treats empty ground tiles as traversable.
     */
    class WalkerPassabilityChecker : public Pathfinding::PassabilityChecker {
        bool isNodeTraversable(
                const PathfindingMap& map, const MapNode& node) const override;
    };

    /**
     * Component that allows an entity to walk around the map.
     */
    class WalkerComponent : public EntityComponent {

    public:
        static const std::string key;

        WalkerComponent();

        // Begin EntityComponent override
        void onEntitySpawned(Scenario* scenario) override;
        void update() override;
        // End EntityComponent override

        void setRoute(Pathfinding::Route route);

    private:
        static WalkerPassabilityChecker passabilityChecker;

        FacingComponent* facingComponent { nullptr };

        Pathfinding::Route route;

        // TMP
        int ticksUntilMove = 10;

        bool canWalk();

        void walkToNextNode();
    };

}  // namespace Rival

#endif  // WALKER_COMPONENT_H
