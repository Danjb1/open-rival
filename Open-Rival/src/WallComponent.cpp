#include "pch.h"
#include "WallComponent.h"

#include "BuildingPropsComponent.h"
#include "Entity.h"
#include "Scenario.h"

namespace Rival {

    const std::string WallComponent::key = "wall";

    WallComponent::WallComponent(WallVariant variant)
        : EntityComponent(key),
          variant(variant) {}

    void WallComponent::onEntitySpawned(Scenario*) {
        int baseTxIndex = 0;

        // Set txIndex based on Wall type
        BuildingPropsComponent* buildingPropsComponent =
                entity->getComponent<BuildingPropsComponent>(
                        BuildingPropsComponent::key);
        if (buildingPropsComponent) {
            baseTxIndex = getBaseTxIndex(
                    buildingPropsComponent->getBuildingType());
        } else {
            // TMP: we need a way to distinguish between Palisades and Grates
            baseTxIndex = baseTxIndexPalisade;
        }

        // Update txIndex in SpriteComponent
        spriteComponent = entity->getComponent<SpriteComponent>(
                SpriteComponent::key);
        if (spriteComponent) {
            int newTxIndex = baseTxIndex + static_cast<int>(variant);
            spriteComponent->setTxIndex(newTxIndex);
        }
    }

    int WallComponent::getBaseTxIndex(Building::Type buildingType) const {
        if (buildingType == Building::Type::TreeWall) {
            return baseTxIndexElf;
        } else if (buildingType == Building::Type::GreenskinWall) {
            return baseTxIndexGreenskin;
        } else {
            return baseTxIndexHuman;
        }
    }

    WallVariant WallComponent::getVariant() const {
        return variant;
    }

}  // namespace Rival
