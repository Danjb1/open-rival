#ifndef FACING_COMPONENT_H
#define FACING_COMPONENT_H

#include "EntityComponent.h"
#include "MapUtils.h"
#include "MovementComponent.h"

namespace Rival {

    struct MapNode;

    /**
     * Interface used to listen to changes in facing.
     */
    class FacingListener {
    public:
        virtual void facingChanged(Facing newFacing) = 0;
    };

    /**
     * Component that manages an Entity's facing.
     */
    class FacingComponent : public EntityComponent,
                            public MovementListener {

    public:
        FacingComponent(Facing initialFacing);

        // Begin EntityComponent override
        void onEntitySpawned(Scenario* scenario) override;
        // End EntityComponent override

        // Begin MovementComponent override
        virtual void onUnitMoveStart(const MapNode* nextNode) override;
        virtual void onUnitJourneyEnd() override;
        // End MovementComponent override

        void setListener(FacingListener* listener);

        void setFacing(Facing newFacing);

        Facing getFacing() const { return facing; }

        void rotateLeft();

        void rotateRight();

    private:
        void notifyListener() const;

    public:
        static const std::string key;

    private:
        static const int numFacings = 8;

        MovementComponent* movementComponent;

        Facing facing;

        FacingListener* listener;
    };

}  // namespace Rival

#endif  // FACING_COMPONENT_H
