#pragma once

#include <glm/vec2.hpp>

#include <map>
#include <string>

#include "entity/components/EntityComponent.h"
#include "game/UnitType.h"
#include "gfx/renderable/SpriteRenderable.h"

namespace Rival {

/**
 * Component that links an Entity to a Renderable.
 *
 * This ensures that all graphical resources are released automatically when an Entity is destroyed.
 *
 * Note that this does not contain any logic to set the txIndex; that must be handled elsewhere.
 */
class SpriteComponent : public EntityComponent
{
public:
    SpriteComponent(const Spritesheet& spritesheet);

    const SpriteRenderable& getRenderable() const;

    int getTxIndex() const;

    void setTxIndex(int txIndex);

public:
    static const std::string key;

    mutable bool dirty = true;
    mutable glm::vec2 lastLerpOffset = { 0, 0 };

private:
    const SpriteRenderable renderable;

    int txIndex = 0;
};

}  // namespace Rival
