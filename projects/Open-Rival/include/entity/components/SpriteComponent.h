#pragma once

#include <glm/vec2.hpp>

#include <map>
#include <memory>
#include <string>

#include "entity/components/EntityComponent.h"
#include "game/UnitType.h"
#include "gfx/Spritesheet.h"

namespace Rival {

class Renderer;

/**
 * Component that links an Entity to a Spritesheet and stores the current texture index.
 */
class SpriteComponent : public EntityComponent
{
public:
    SpriteComponent(std::shared_ptr<const Spritesheet> spritesheet, float offsetY = 0.f);

    std::shared_ptr<const Spritesheet> getSpritesheet() const
    {
        return spritesheet;
    }

    int getTxIndex() const;

    void setTxIndex(int txIndex);

    float getOffsetY() const
    {
        return offsetY;
    }

public:
    static const std::string key;

    /** Flag set whenever the texture index changes. */
    mutable bool dirty = true;

    /** Last lerp offset used during rendering. */
    mutable glm::vec2 lastLerpOffset = { 0, 0 };

private:
    std::shared_ptr<const Spritesheet> spritesheet;
    float offsetY = 0.f;
    int txIndex = 0;
};

}  // namespace Rival
