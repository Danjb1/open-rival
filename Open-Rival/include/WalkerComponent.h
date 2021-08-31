#ifndef WALKER_COMPONENT_H
#define WALKER_COMPONENT_H

#include <vector>

#include "EntityComponent.h"
#include "Pathfinding.h"

namespace Rival {

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

        void setRoute(std::vector<Pathfinding::Node> route);

    private:
        std::vector<Pathfinding::Node> route;

        // TMP
        int delay = 0;

        bool canWalk();

        void walkToNextNode();
    };

}  // namespace Rival

#endif  // WALKER_COMPONENT_H
