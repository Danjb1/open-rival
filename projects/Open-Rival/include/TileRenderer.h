#pragma once

#define GLEW_STATIC
#include <gl/glew.h>

#include <memory>
#include <vector>

#include "RenderUtils.h"
#include "SpriteRenderable.h"
#include "Tile.h"

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

    void render(const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight) const;

private:
    SpriteRenderable renderable;

    std::shared_ptr<const Texture> paletteTexture;

    bool needsUpdate() const;

    void sendDataToGpu(const Camera& camera, const std::vector<Tile>& tiles, int mapWidth, int mapHeight) const;
};

}  // namespace Rival
