#include "pch.h"
#include "FacingComponent.h"

namespace Rival {

    const std::string FacingComponent::key = "facing";

    FacingComponent::FacingComponent(Facing initialFacing)
        : EntityComponent(key),
          facing(initialFacing) {}

    void FacingComponent::setListener(FacingListener* newListener) {
        listener = newListener;
    }

    void FacingComponent::setFacing(Facing newFacing) {
        facing = newFacing;
        notifyListener();
    }

    void FacingComponent::rotateLeft() {
        int newFacing = (static_cast<int>(facing) - 1) % numFacings;
        facing = static_cast<Facing>(newFacing);
    }

    void FacingComponent::rotateRight() {
        int newFacing = (static_cast<int>(facing) + 1) % numFacings;
        facing = static_cast<Facing>(newFacing);
    }

    void FacingComponent::notifyListener() const {
        if (listener) {
            listener->facingChanged(facing);
        }
    }

}  // namespace Rival
