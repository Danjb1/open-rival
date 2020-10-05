#ifndef SCENARIO_BUILDER_H
#define SCENARIO_BUILDER_H

#include <cstdint>
#include <fstream>
#include <memory>

#include "Building.h"
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

        std::unique_ptr<Scenario> build(const Resources& res);

    private:
        ScenarioData data;

        Tile buildTile(TilePlacement& tile) const;

        Building::Type getBuildingType(std::uint8_t buildingType) const;

        Unit::Type getUnitType(std::uint8_t unitType) const;

        Facing getFacing(std::uint8_t facing) const;
    };

}  // namespace Rival

#endif  // SCENARIO_BUILDER_H
