#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <memory>

#include "Building.h"
#include "Entity.h"
#include "FacingComponent.h"
#include "Resources.h"
#include "ScenarioData.h"
#include "Unit.h"

namespace Rival {

    class EntityFactory {

    public:
        EntityFactory(const Resources& res);

        /**
         * Creates a Unit from raw data (e.g. read from a Scenario file).
         */
        std::shared_ptr<Entity> createUnit(
                const UnitPlacement& unitPlacement) const;

        /**
         * Creates a Building from raw data (e.g. read from a Scenario file).
         */
        std::shared_ptr<Entity> createBuilding(
                const BuildingPlacement& buildingPlacement) const;

        /**
         * Creates a Palisade from raw data (e.g. read from a Scenario file).
         */
        std::shared_ptr<Entity> createPalisade(
                const BuildingPlacement& buildingPlacement,
                bool wilderness) const;

        /**
         * Creates an Object from raw data (e.g. read from a Scenario file).
         */
        std::shared_ptr<Entity> EntityFactory::createObject(
                const ObjectPlacement& objPlacement, bool wilderness) const;

    private:
        const Resources& res;

        Building::Type getBuildingType(std::uint8_t buildingType) const;

        Unit::Type getUnitType(std::uint8_t unitType) const;

        Facing getFacing(std::uint8_t facing) const;
    };

}  // namespace Rival

#endif  // ENTITY_FACTORY_H
