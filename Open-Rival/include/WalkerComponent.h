#ifndef WALKER_COMPONENT_H
#define WALKER_COMPONENT_H

#include <vector>

#include "EntityComponent.h"
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
        virtual void update() override;
        // End EntityComponent override

        void setRoute(Pathfinding::Route route);

    private:
        static WalkerPassabilityChecker passabilityChecker;

        Pathfinding::Route route;

        // TMP
        int delay = 0;

        bool canWalk();

        void walkToNextNode();
    };

}  // namespace Rival

#endif  // WALKER_COMPONENT_H
