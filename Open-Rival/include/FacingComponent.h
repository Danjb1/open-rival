#ifndef FACING_COMPONENT_H
#define FACING_COMPONENT_H

#include "EntityComponent.h"

namespace Rival {

    enum class Facing : std::uint8_t {
        South,
        SouthWest,
        West,
        NorthWest,
        North,
        NorthEast,
        East,
        SouthEast
    };

    class FacingComponent : public EntityComponent {

    public:
        static const std::string key;

        Facing facing;

        FacingComponent(Facing initialFacing);

        void rotateLeft();

        void rotateRight();

    private:
        static const int numFacings = 8;
    };

}  // namespace Rival

#endif  // FACING_COMPONENT_H
