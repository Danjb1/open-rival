#pragma once

#include <memory>

#include "Building.h"
#include "Entity.h"
#include "MapUtils.h"
#include "PlayerState.h"
#include "ScenarioData.h"
#include "Unit.h"

namespace Rival {

class AudioStore;
class AudioSystem;
class DataStore;
class TextureStore;

class EntityFactory
{
public:
    EntityFactory(
            const DataStore& dataStore,
            const TextureStore& textureStore,
            const AudioStore& audioStore,
            AudioSystem& audioSystem);

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
    std::shared_ptr<Entity> EntityFactory::createObject(const ObjectPlacement& objPlacement, bool wilderness) const;

private:
    Building::Type getBuildingType(std::uint8_t buildingType) const;

    Unit::Type getUnitType(std::uint8_t unitType) const;

    Facing getFacing(std::uint8_t facing) const;

private:
    const AudioStore& audioStore;
    const DataStore& dataStore;
    const TextureStore& textureStore;

    AudioSystem& audioSystem;
};

}  // namespace Rival
