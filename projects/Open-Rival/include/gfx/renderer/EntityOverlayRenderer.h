#pragma once

#include "gfx/GlewWrapper.h"

#include <vector>

#include "gfx/renderable/BoxRenderable.h"
#include "utils/EntityUtils.h"

namespace Rival {

class Camera;

/**
 * Class responsible for rendering the health/mana/XP bars above Entities.
 */
class EntityOverlayRenderer
{
public:
    EntityOverlayRenderer();

    // Prevent copying
    EntityOverlayRenderer(const EntityOverlayRenderer&) = delete;
    EntityOverlayRenderer& operator=(const EntityOverlayRenderer&) = delete;

    void render(const EntityContainer& entityContainer);

private:
    void addEntityOverlayToBuffers(const Entity& entity, int& numBoxes);

private:
    // Unlikely we'll need more than this
    static constexpr int maxBoxesToRender = 256;

    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> colorData;
    BoxRenderable boxRenderable;
};

}  // namespace Rival
