#pragma once

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
    class FacingComponent
        : public EntityComponent
        , public MovementListener {

    public:
        FacingComponent(Facing initialFacing);

        // Begin EntityComponent override
        virtual void onEntitySpawned(Scenario* scenario) override;
        virtual void onDelete() override;
        // End EntityComponent override

        // Begin MovementComponent override
        virtual void onUnitMoveStart(const MapNode* nextNode) override;
        virtual void onUnitJourneyEnd() override;
        // End MovementComponent override

        void setListener(FacingListener* listener);

        void setFacing(Facing newFacing);

        Facing getFacing() const {
            return facing;
        }

        void rotateLeft();

        void rotateRight();

    private:
        void notifyListener() const;

    public:
        static const std::string key;

    private:
        static const int numFacings = 8;

        std::weak_ptr<MovementComponent> weakMovementComponent;

        Facing facing;

        FacingListener* listener { nullptr };
    };

}  // namespace Rival
