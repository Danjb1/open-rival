#include "pch.h"
#include "FacingComponent.h"

namespace Rival {

    const std::string FacingComponent::key = "facing";

    FacingComponent::FacingComponent(Facing initialFacing)
        : EntityComponent(key),
          facing(initialFacing) {}

    void FacingComponent::rotateLeft() {
        int newFacing = (static_cast<int>(facing) - 1) % numFacings;
        facing = static_cast<Facing>(newFacing);
    }

    void FacingComponent::rotateRight() {
        int newFacing = (static_cast<int>(facing) + 1) % numFacings;
        facing = static_cast<Facing>(newFacing);
    }

}  // namespace Rival
