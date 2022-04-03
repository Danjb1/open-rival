#ifndef FACING_COMPONENT_H
#define FACING_COMPONENT_H

#include "EntityComponent.h"
#include "MapUtils.h"

namespace Rival {

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
    class FacingComponent : public EntityComponent {

    public:
        static const std::string key;

        FacingComponent(Facing initialFacing);

        void setListener(FacingListener* listener);

        void setFacing(Facing newFacing);

        Facing getFacing() const { return facing; }

        void rotateLeft();

        void rotateRight();

    private:
        static const int numFacings = 8;

        Facing facing;

        FacingListener* listener;

        void notifyListener() const;
    };

}  // namespace Rival

#endif  // FACING_COMPONENT_H
