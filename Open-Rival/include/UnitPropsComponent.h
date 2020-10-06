#ifndef UNIT_PROPS_COMPONENT_H
#define UNIT_PROPS_COMPONENT_H

#include "EntityComponent.h"
#include "Unit.h"

namespace Rival {

    class UnitPropsComponent : public EntityComponent {

    public:
        static const std::string key;

        UnitPropsComponent(Unit::Type type);

        Unit::Type getUnitType() const;

    private:
        Unit::Type type;
    };

}  // namespace Rival

#endif  // UNIT_PROPS_COMPONENT_H
