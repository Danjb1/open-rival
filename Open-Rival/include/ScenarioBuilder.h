#ifndef SCENARIO_BUILDER_H
#define SCENARIO_BUILDER_H

#include <cstdint>
#include <fstream>

#include "Scenario.h"
#include "ScenarioData.h"

namespace Rival {

    // Class that can create a Scenario from previously-loaded ScenarioData
    class ScenarioBuilder {

    public:

        ScenarioBuilder(ScenarioData data);

        std::unique_ptr<Scenario> build();

    private:

        ScenarioData data;

        Tile buildTile(TilePlacement& tile) const;

    };

}

#endif // SCENARIO_BUILDER_H
