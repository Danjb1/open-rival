#pragma once

#include <cstdint>
#include <fstream>
#include <memory>

#include "game/Race.h"
#include "game/Tile.h"
#include "scenario/ScenarioData.h"

namespace Rival {

class EntityFactory;
class World;

// Class that can create a World from previously-loaded ScenarioData
class ScenarioBuilder
{
public:
    ScenarioBuilder(ScenarioData data);

    std::unique_ptr<World> build(const EntityFactory& entityFactory);

    Race getRace(std::uint8_t raceId) const;

private:
    ScenarioData data;

    Tile buildTile(TilePlacement& tile) const;

    void addUnit(World* world, const UnitPlacement& unitPlacement, const EntityFactory& entityFactory) const;

    void addPalisade(
            World* world, const BuildingPlacement& buildingPlacement, const EntityFactory& entityFactory) const;

    void addGrate(World* world, const BuildingPlacement& buildingPlacement, const EntityFactory& entityFactory) const;

    void addBuilding(
            World* world, const BuildingPlacement& buildingPlacement, const EntityFactory& entityFactory) const;

    void addObject(World* world, const ObjectPlacement& objPlacement, const EntityFactory& entityFactory) const;
};

}  // namespace Rival
