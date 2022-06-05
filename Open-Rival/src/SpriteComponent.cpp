#include "pch.h"
#include "SpriteComponent.h"

#include "Entity.h"

namespace Rival {

    const std::string SpriteComponent::key = "sprite";

    SpriteComponent::SpriteComponent(const Spritesheet& spritesheet)
        : EntityComponent(key)
        , txIndex(0)
        , dirty(true)
        , renderable(spritesheet, 1)
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
