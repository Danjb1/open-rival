#pragma once

#include "gfx/GlewWrapper.h"

#include <vector>

#include "gfx/RenderUtils.h"
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

    static constexpr int healthColorIndex1 = 172;
    static constexpr int healthColorIndex2 = 176;

    // TODO: These were measured for a Centaur with 2 bars; should they vary based on unit hitbox / number of bars?
    static constexpr int healthBarDrawOffsetX = 19;
    static constexpr int healthBarDrawOffsetY = -43;

    static constexpr float healthBarWidth = 25.f;
    static constexpr float healthBarHeight = 3.f;

    std::vector<GLfloat> healthColor1;
    std::vector<GLfloat> healthColor2;

    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> colorData;
    BoxRenderable boxRenderable;
};

}  // namespace Rival
