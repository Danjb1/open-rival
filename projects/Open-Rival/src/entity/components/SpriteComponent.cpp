#include "entity/components/SpriteComponent.h"

#include "entity/Entity.h"

namespace Rival {

const std::string SpriteComponent::key = "sprite";

SpriteComponent::SpriteComponent(std::shared_ptr<const Spritesheet> spritesheet, float offsetY)
    : EntityComponent(key)
    , renderable(spritesheet, 1)
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

const SpriteRenderable& SpriteComponent::getRenderable() const
{
    return renderable;
}

}  // namespace Rival
