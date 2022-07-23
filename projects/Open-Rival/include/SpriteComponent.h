#pragma once

#include <glm/vec2.hpp>

#include <map>
#include <string>

#include "EntityComponent.h"
#include "SpriteRenderable.h"
#include "Unit.h"

namespace Rival {

/**
 * Component that links an Entity to a Renderable.
 *
 * This ensures that all graphical resources are released automatically
 * when an Entity is destroyed.
 *
 * Note that this does not contain any logic to set the txIndex; that must
 * be handled elsewhere.
 */
class SpriteComponent : public EntityComponent
{

public:
    static const std::string key;

    mutable bool dirty;
    mutable glm::vec2 lastLerpOffset;

    SpriteComponent(const Spritesheet& spritesheet);

    const SpriteRenderable& getRenderable() const;

    int getTxIndex() const;

    void setTxIndex(int txIndex);

private:
    const SpriteRenderable renderable;

    int txIndex;
};

}  // namespace Rival
