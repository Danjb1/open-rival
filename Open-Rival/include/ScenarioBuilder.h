#ifndef SCENARIO_BUILDER_H
#define SCENARIO_BUILDER_H

#include <cstdint>
#include <fstream>
#include <memory>

#include "Building.h"
#include "EntityFactory.h"
#include "FacingComponent.h"
#include "Resources.h"
#include "Scenario.h"
#include "ScenarioData.h"
#include "Tile.h"
#include "Unit.h"

namespace Rival {

    // Class that can create a Scenario from previously-loaded ScenarioData
    class ScenarioBuilder {

    public:
        ScenarioBuilder(ScenarioData data);

        std::unique_ptr<Scenario> build(const EntityFactory& entityFactory);

    private:
        ScenarioData data;

        Tile buildTile(TilePlacement& tile) const;

        void addUnit(
                Scenario* scenario,
                const UnitPlacement& unitPlacement,
                const EntityFactory& entityFactory) const;

        void addPalisade(
                Scenario* scenario,
                const BuildingPlacement& buildingPlacement,
                const EntityFactory& entityFactory) const;

        void addBuilding(
                Scenario* scenario,
                const BuildingPlacement& buildingPlacement,
                const EntityFactory& entityFactory) const;

        void addObject(
                Scenario* scenario,
                const ObjectPlacement& objPlacement,
                const EntityFactory& entityFactory) const;
    };

}  // namespace Rival

#endif  // SCENARIO_BUILDER_H
