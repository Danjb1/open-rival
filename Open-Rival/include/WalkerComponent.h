#ifndef WALKER_COMPONENT_H
#define WALKER_COMPONENT_H

#include <vector>

#include "EntityComponent.h"
#include "FacingComponent.h"
#include "Pathfinding.h"
#include "UnitPropsComponent.h"

namespace Rival {

    /**
     * PassabilityChecker that treats empty ground tiles as traversable.
     */
    class WalkerPassabilityChecker : public Pathfinding::PassabilityChecker {
        bool isNodeTraversable(
                const PathfindingMap& map, const MapNode& node) const override;
    };

    /**
     * Represents a movement between 2 tiles.
     */
    struct Movement {
        /**
         * Time spent moving so far, in ms.
         */
        int timeElapsed;

        /**
         * Total time required to complete the movement, in ms.
         */
        int timeRequired;
    };

    /**
     * Component that allows an entity to walk around the map.
     *
     * Note that during movement, entities are considered to occupy their
     * original tile until they have fully moved into the new tile.
     */
    class WalkerComponent : public EntityComponent {

    public:
        static const std::string key;

        WalkerComponent();

        // Begin EntityComponent override
        void onEntitySpawned(Scenario* scenario) override;
        void update() override;
        // End EntityComponent override

        /**
         * Sets the current route.
         */
        void setRoute(Pathfinding::Route route);

        /**
         * Gets the current route.
         */
        const Pathfinding::Route getRoute() const { return route; }

        /**
         * Gets the movement that's currently in progress.
         */
        const Movement& getMovement() const { return movement; }

    private:
        static WalkerPassabilityChecker passabilityChecker;

        UnitPropsComponent* unitPropsComponent { nullptr };
        FacingComponent* facingComponent { nullptr };

        Pathfinding::Route route;

        Movement movement;

        // TMP
        int ticksPerMove = 30;

        void prepareNextMovement();
        void completeMovement();
    };

}  // namespace Rival

#endif  // WALKER_COMPONENT_H
