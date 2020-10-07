#include "pch.h"
#include "WallComponent.h"

#include "BuildingPropsComponent.h"
#include "Entity.h"

namespace Rival {

    const std::string WallComponent::key = "wall";

    WallComponent::WallComponent(WallVariant variant)
        : EntityComponent(key),
          variant(variant) {}

    void WallComponent::onEntitySpawned() {

        int baseTxIndex = 0;

        // Set txIndex based on Wall type
        BuildingPropsComponent* buildingPropsComponent =
                dynamic_cast<BuildingPropsComponent*>(
                        entity->getComponent(BuildingPropsComponent::key));
        if (buildingPropsComponent) {
            baseTxIndex = getBaseTxIndex(
                    buildingPropsComponent->getBuildingType());
        }

        // Update txIndex in SpriteComponent
        spriteComponent = dynamic_cast<SpriteComponent*>(
                entity->getComponent(SpriteComponent::key));
        if (spriteComponent) {
            spriteComponent->txIndex = baseTxIndex + static_cast<int>(variant);
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
