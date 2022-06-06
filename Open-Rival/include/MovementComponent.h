#pragma once

#include <unordered_set>

#include "EntityComponent.h"

namespace Rival {

    struct MapNode;

    /**
     * Represents a movement between 2 tiles.
     */
    struct Movement
    {
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
     * Interface used to listen to movements.
     */
    class MovementListener
    {
    public:
        virtual void onUnitMoveStart(const MapNode* nextNode) = 0;
        virtual void onUnitJourneyEnd() = 0;
    };

    /**
     * Base class for a component that allows entities to move between tiles.
     */
    class MovementComponent : public EntityComponent
    {

    public:
        MovementComponent();
        virtual ~MovementComponent() = default;

        void addListener(MovementListener* listener);
        void removeListener(MovementListener* listener);

    public:
        static const std::string key;

    protected:
        std::unordered_set<MovementListener*> listeners;
    };

}  // namespace Rival
