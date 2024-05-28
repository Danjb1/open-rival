#pragma once

#include "gfx/GlewWrapper.h"

#include <string>
#include <vector>

#include "gfx/RenderUtils.h"
#include "gfx/renderable/AtlasRenderable.h"
#include "utils/EntityUtils.h"

namespace Rival {

class Camera;
class Texture;
class TextureStore;

/**
 * Class responsible for rendering the health/mana/XP bars above Entities.
 */
class EntityOverlayRenderer
{
public:
    EntityOverlayRenderer(const TextureStore& textureStore);

    // Prevent copying
    EntityOverlayRenderer(const EntityOverlayRenderer&) = delete;
    EntityOverlayRenderer& operator=(const EntityOverlayRenderer&) = delete;

    void render(const EntityContainer& entityContainer);

private:
    void addEntityOverlayToBuffers(const Entity& entity, int& numSprites);

private:
    // TODO: This is an arbitrary value but we could run out!
    static constexpr int maxImagesToRender = 1024;

    // TODO: These were measured for a Centaur with 2 bars; should they vary based on unit hitbox / number of bars?
    static constexpr int healthBarDrawOffsetX = 19;
    static constexpr int healthBarDrawOffsetY = -43;

    static constexpr float healthBarWidth = 25.f;
    static constexpr float healthBarHeight = 3.f;

    static const std::string healthBarAtlasKey;
    static const std::string healthBarDepletedAtlasKey;
    static const std::string monsterHealthBarAtlasKey;
    static const std::string monsterHealthBarDepletedAtlasKey;

    std::shared_ptr<const Texture> paletteTexture;

    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> texCoordData;
    AtlasRenderable overlayRenderable;
};

}  // namespace Rival
