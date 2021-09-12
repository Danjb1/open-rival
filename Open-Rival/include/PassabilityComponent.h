#ifndef PASSABILITY_COMPONENT_H
#define PASSABILITY_COMPONENT_H

#include "EntityComponent.h"
#include "Scenario.h"
#include "Tile.h"

namespace Rival {

    /**
     * Component that affects the passability of an entity's tile.
     */
    class PassabilityComponent : public EntityComponent {

    public:
        static const std::string key;

        PassabilityComponent(TilePassability passability);

        void onEntitySpawned(Scenario* scenario) override;

        TilePassability getPassability() const;

    private:
        TilePassability passability;
    };

}  // namespace Rival

#endif  // PASSABILITY_COMPONENT_H
