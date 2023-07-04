#pragma once

#include "gfx/GlewWrapper.h"

#include <memory>
#include <vector>

#include "game/Tile.h"
#include "gfx/RenderUtils.h"
#include "gfx/renderable/SpriteRenderable.h"

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

    TileRenderer(const Spritesheet& spritesheet, std::shared_ptr<const Texture> paletteTexture);

    void render(int delta, const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight);

private:
    void updateWaterPalettes(int delta);
    bool needsUpdate() const;
    void sendDataToGpu(const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight) const;

private:
    static constexpr int msPerWaterShift = 180;
    static constexpr int waterPalette1Size = 4;
    static constexpr int waterPalette2Size = 7;

    SpriteRenderable renderable;
    std::shared_ptr<const Texture> paletteTexture;

    int waterPaletteShift1 = 0;
    int waterPaletteShift2 = 0;
    int msSinceWaterShift = 0;
};

}  // namespace Rival
