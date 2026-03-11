#include "entity/components/SpriteComponent.h"

#include "entity/Entity.h"

namespace Rival {

const std::string SpriteComponent::key = "sprite";

SpriteComponent::SpriteComponent(std::shared_ptr<const Spritesheet> spritesheet, float offsetY)
    : EntityComponent(key)
    , spritesheet(spritesheet)
    , offsetY(offsetY)
{
}

int SpriteComponent::getTxIndex() const
{
    return txIndex;
}

void SpriteComponent::setTxIndex(int newTxIndex)
{
    txIndex = newTxIndex;
    dirty = true;
}

}  // namespace Rival
