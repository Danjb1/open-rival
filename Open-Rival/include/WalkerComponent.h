#ifndef WALKER_COMPONENT_H
#define WALKER_COMPONENT_H

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

        // Begin EntityComponent override
        void update() override;
        // End EntityComponent override

        /**
         * Sets the current route.
         */
        void setRoute(Pathfinding::Route route);

        /**
         * Gets the current route.
         */
        const Pathfinding::Route getRoute() const
        {
            return route;
        }

        /**
         * Gets the movement that's currently in progress.
         */
        const Movement& getMovement() const
        {
            return movement;
        }

    private:
        void prepareNextMovement();
        void completeMovement();

    private:
        static WalkerPassabilityChecker passabilityChecker;

        Pathfinding::Route route;

        Movement movement;

        // TMP: This should depend on the unit's speed
        int ticksPerMove = 30;
    };

}  // namespace Rival

#endif  // WALKER_COMPONENT_H
