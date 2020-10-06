#include "pch.h"
#include "SpriteComponent.h"

#include "Entity.h"

namespace Rival {

    const std::string SpriteComponent::key = "sprite";

    SpriteComponent::SpriteComponent(
            const Spritesheet& spritesheet)
        : EntityComponent(key),
          txIndex(0),
          renderable(spritesheet, 1) {}

    int SpriteComponent::getTxIndex() const {
        return txIndex;
    }

    const SpriteRenderable& SpriteComponent::getRenderable() const {
        return renderable;
    }

}  // namespace Rival
