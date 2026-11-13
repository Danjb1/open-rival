#pragma once

#include "gfx/opengl/GLWrapper.h"

#include <array>
#include <memory>
#include <vector>

#include "game/Tile.h"
#include "gfx/RenderConstants.h"
#include "gfx/opengl/renderable/SpriteRenderable.h"

namespace Rival {

class Camera;
class Texture;
class Spritesheet;

/**
 * Class responsible for rendering Tiles.
 *
 * It renders all Tiles in a single draw operation.
 */
class TileRenderer
{

public:
    // The maximum number of tiles we can render at once
    static constexpr int maxTilesToRender = RenderUtils::maxTilesX * RenderUtils::maxTilesY;

    TileRenderer(std::shared_ptr<const Spritesheet> spritesheet, std::shared_ptr<const Texture> paletteTexture);

    void render(float timeSinceLastRender, const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight);

private:
    void updateWaterPalettes(float timeSinceLastRender);
    bool needsUpdate() const;
    void sendDataToGpu(const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight) const;

private:
    static constexpr float timePerWaterFrame = 0.18f;
    static const std::array<int, 4> waterFrames1;
    static const std::array<int, 7> waterFrames2;

    SpriteRenderable renderable;
    std::shared_ptr<const Texture> paletteTexture;

    int waterIndex1 = 0;
    int waterIndex2 = 0;
    float timeSinceWaterShift = 0.f;
};

}  // namespace Rival
