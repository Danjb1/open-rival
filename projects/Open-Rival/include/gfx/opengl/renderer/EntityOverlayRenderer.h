#pragma once

#include "gfx/opengl/GLWrapper.h"
#include <glm/vec4.hpp>

#include <string>
#include <vector>

#include "gfx/opengl/renderable/AtlasRenderable.h"
#include "gfx/opengl/renderable/BoxRenderable.h"
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

    static constexpr float healthBarDrawOffsetX = 19;
    static constexpr float healthBarDrawOffsetY = 11;

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

    glm::vec4 backgroundColor;
    std::vector<Box> boxes;
    BoxRenderable boxRenderable;

    std::vector<AtlasImage> images;
    AtlasRenderable overlayRenderable;
};

}  // namespace Rival
