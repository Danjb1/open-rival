#pragma once

#include <memory>

#include "Building.h"
#include "Entity.h"
#include "GameCommand.h"
#include "MapUtils.h"
#include "PlayerState.h"
#include "ScenarioData.h"
#include "Unit.h"

namespace Rival {

class AudioSystem;
class Resources;

class EntityFactory
{
public:
    EntityFactory(const Resources& resources, AudioSystem& audioSystem);

    /**
     * Creates a Unit from raw data (e.g. read from a Scenario file).
     */
    std::shared_ptr<Entity> createUnit(const UnitPlacement& unitPlacement) const;

    /**
     * Creates a Building from raw data (e.g. read from a Scenario file).
     */
    std::shared_ptr<Entity> createBuilding(const BuildingPlacement& buildingPlacement) const;

    /**
     * Creates a Palisade from raw data (e.g. read from a Scenario file).
     */
    std::shared_ptr<Entity> createPalisade(const BuildingPlacement& buildingPlacement, bool wilderness) const;

    /**
     * Creates an Object from raw data (e.g. read from a Scenario file).
     */
    std::shared_ptr<Entity> createObject(const ObjectPlacement& objPlacement, bool wilderness) const;

private:
    Unit::Type getUnitType(std::uint8_t unitType) const;
    bool isNameEmpty(const std::string& name) const;
    Facing getFacing(std::uint8_t facing) const;
    Building::Type getBuildingType(std::uint8_t buildingType) const;

private:
    const Resources& resources;
    AudioSystem& audioSystem;
};

}  // namespace Rival
