#include "pch.h"
#include "SpriteComponent.h"

#include "Entity.h"

namespace Rival {

    const std::string SpriteComponent::key = "sprite";

    SpriteComponent::SpriteComponent(
            const Spritesheet& spritesheet)
        : EntityComponent(key),
          renderable(spritesheet, 1) {}

    int SpriteComponent::getTxIndex() const {
        // TODO
        return 0;
    }

    const SpriteRenderable& SpriteComponent::getRenderable() const {
        return renderable;
    }

}  // namespace Rival
