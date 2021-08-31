#ifndef WALL_COMPONENT_H
#define WALL_COMPONENT_H

#include "Building.h"
#include "EntityComponent.h"
#include "SpriteComponent.h"

namespace Rival {

    // We have 16 wall variants.
    // The underlying int value corresponds to the wall's texture index.
    enum class WallVariant : std::uint8_t {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P
    };

    /**
     * Component containing data required by wall segments.
     */
    class WallComponent : public EntityComponent {

    public:
        static const std::string key;

        WallComponent(WallVariant variant);

        void onEntitySpawned(Scenario* scenario) override;

        WallVariant getVariant() const;

    private:
        static const int baseTxIndexElf = 48;
        static const int baseTxIndexGreenskin = 33;
        static const int baseTxIndexHuman = 42;
        static const int baseTxIndexPalisade = 32;
        static const int baseTxIndexGrate = 48;

        SpriteComponent* spriteComponent;

        WallVariant variant;

        int getBaseTxIndex(Building::Type buildingType) const;
    };

}  // namespace Rival

#endif  // WALL_COMPONENT_H
