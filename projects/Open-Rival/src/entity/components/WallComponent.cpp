#include "entity/components/WallComponent.h"

#include "entity/Building.h"
#include "entity/Entity.h"
#include "entity/Palisade.h"
#include "game/World.h"
#include "utils/LogUtils.h"

namespace Rival {

const std::string WallComponent::key = "wall";

WallComponent::WallComponent(WallVariant variant)
    : EntityComponent(key)
    , variant(variant)
{
}

void WallComponent::onEntityAddedToWorld(World*)
{
    // Set txIndex based on Wall type
    int baseTxIndex = 0;
    if (Building* building = entity->as<Building>())
    {
        baseTxIndex = getBaseTxIndex(building->getBuildingType());
    }
    else if (Palisade* palisade = entity->as<Palisade>())
    {
        baseTxIndex = baseTxIndexPalisade;
    }
    else
    {
        baseTxIndex = baseTxIndexGrate;
    }

    // Update txIndex in SpriteComponent
    SpriteComponent* spriteComponent = entity->requireComponent<SpriteComponent>(SpriteComponent::key);
    if (spriteComponent)
    {
        int newTxIndex = baseTxIndex + static_cast<int>(variant);
        spriteComponent->setTxIndex(newTxIndex);
    }
}

int WallComponent::getBaseTxIndex(BuildingType buildingType) const
{
    if (buildingType == BuildingType::TreeWall)
    {
        return baseTxIndexElf;
    }
    else if (buildingType == BuildingType::GreenskinWall)
    {
        return baseTxIndexGreenskin;
    }
    else
    {
        return baseTxIndexHuman;
    }
}

WallVariant WallComponent::getVariant() const
{
    return variant;
}

}  // namespace Rival
