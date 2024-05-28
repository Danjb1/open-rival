#pragma once

#include "gfx/GlewWrapper.h"

#include <string>
#include <vector>

#include "gfx/RenderUtils.h"
#include "gfx/renderable/AtlasRenderable.h"
#include "gfx/renderable/BoxRenderable.h"
#include "utils/EntityUtils.h"

namespace Rival {

class Camera;
class Texture;
class TextureStore;
struct PlayerContext;

/**
 * Class responsible for rendering the health/mana/XP bars above Entities.
 */
class EntityOverlayRenderer
{
public:
    EntityOverlayRenderer(const TextureStore& textureStore, const PlayerContext& playerContext);

    // Prevent copying
    EntityOverlayRenderer(const EntityOverlayRenderer&) = delete;
    EntityOverlayRenderer& operator=(const EntityOverlayRenderer&) = delete;

    /** Prepares for rendering.
     * This should always be called before calling renderBoxes / renderTextures. */
    void prepare(const EntityContainer& entityContainer);

    void renderBoxes();
    void renderTextures();

private:
    void addEntityOverlayToBuffers(const Entity& entity, bool isHovered);

private:
    // 2 x health bar + depleted health bar
    static constexpr int maxSpritesPerEntity = 3;

    // It's very unlikely we'll need to render more than this!
    static constexpr int maxOverlaysToRender = 512;
    static constexpr int maxImagesToRender = maxOverlaysToRender * maxSpritesPerEntity;

    // TODO: These were measured for a Centaur with 2 bars; should they vary based on unit hitbox / number of bars?
    static constexpr int healthBarDrawOffsetX = 19;
    static constexpr int healthBarDrawOffsetY = -43;

    static constexpr float healthBarWidth = 25.f;
    static constexpr float healthBarHeight = 3.f;
    static constexpr int healthPerBar = 300;

    static const std::string overlayBackgroundAtlasKey;
    static const std::string healthBarAtlasKey;
    static const std::string healthBarDepletedAtlasKey;
    static const std::string monsterHealthBarAtlasKey;
    static const std::string monsterHealthBarDepletedAtlasKey;

    const PlayerContext& playerContext;

    std::shared_ptr<const Texture> paletteTexture;

    std::vector<GLfloat> backgroundColor;
    std::vector<GLfloat> boxVertexData;
    std::vector<GLfloat> boxColorData;
    BoxRenderable boxRenderable;

    std::vector<GLfloat> atlasVertexData;
    std::vector<GLfloat> atlasTexCoordData;
    AtlasRenderable overlayRenderable;

    int numBoxes = 0;
    int numSprites = 0;
};

}  // namespace Rival
