#pragma once

#include <memory>

#include "game/BuildingType.h"
#include "game/MapUtils.h"
#include "game/UnitType.h"
#include "scenario/ScenarioData.h"

namespace Rival {

class AudioSystem;
class Entity;
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
    std::shared_ptr<Entity> createPalisade(
            const BuildingPlacement& buildingPlacement, bool wilderness, bool isGrate) const;

    /**
     * Creates an Object from raw data (e.g. read from a Scenario file).
     */
    std::shared_ptr<Entity> createObject(const ObjectPlacement& objPlacement, bool wilderness) const;

private:
    UnitType getUnitType(std::uint8_t unitType) const;
    bool isNameEmpty(const std::string& name) const;
    Facing getFacing(std::uint8_t facing) const;
    BuildingType getBuildingType(std::uint8_t buildingType) const;

private:
    const Resources& resources;
    AudioSystem& audioSystem;
};

}  // namespace Rival
