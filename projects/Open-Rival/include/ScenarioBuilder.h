#pragma once

#include <cstdint>
#include <fstream>
#include <memory>

#include "Building.h"
#include "EntityFactory.h"
#include "FacingComponent.h"
#include "Resources.h"
#include "ScenarioData.h"
#include "Tile.h"
#include "Unit.h"
#include "World.h"

namespace Rival {

// Class that can create a World from previously-loaded ScenarioData
class ScenarioBuilder
{

public:
    ScenarioBuilder(ScenarioData data);

    std::unique_ptr<World> build(const EntityFactory& entityFactory);

private:
    ScenarioData data;

    Tile buildTile(TilePlacement& tile) const;

    void addUnit(World* scenario, const UnitPlacement& unitPlacement, const EntityFactory& entityFactory) const;

    void
    addPalisade(World* scenario, const BuildingPlacement& buildingPlacement, const EntityFactory& entityFactory) const;

    void
    addBuilding(World* scenario, const BuildingPlacement& buildingPlacement, const EntityFactory& entityFactory) const;

    void addObject(World* scenario, const ObjectPlacement& objPlacement, const EntityFactory& entityFactory) const;
};

}  // namespace Rival
