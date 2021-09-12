#ifndef UNIT_PROPS_COMPONENT_H
#define UNIT_PROPS_COMPONENT_H

#include <cstdint>  // uint8_t

#include "EntityComponent.h"
#include "Unit.h"

namespace Rival {

    enum class UnitState : std::uint8_t {
        Idle,
        Moving,
        Attacking
    };

    /**
     * Component containing basic unit properties.
     */
    class UnitPropsComponent : public EntityComponent {

    public:
        static const std::string key;

        UnitPropsComponent(Unit::Type type);

        Unit::Type getUnitType() const { return type; }

        /**
         * Gets the unit's current state.
         */
        UnitState getState() const { return state; }

        /**
         * Sets this Entity's state.
         */
        void setState(UnitState state);

    private:
        Unit::Type type;

        UnitState state = UnitState::Idle;
    };

}  // namespace Rival

#endif  // UNIT_PROPS_COMPONENT_H
