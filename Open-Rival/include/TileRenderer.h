#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include <gl/glew.h>
#include <vector>

#include "Camera.h"
#include "RenderUtils.h"
#include "Spritesheet.h"
#include "SpriteRenderable.h"
#include "Texture.h"
#include "Tile.h"

namespace Rival {

    /**
     * Class responsible for rendering Tiles.
     *
     * It renders all Tiles in a single draw operation.
     */
    class TileRenderer {

    public:
        // The maximum number of tiles we can render at once
        static const int maxTilesToRender =
                RenderUtils::maxTilesX * RenderUtils::maxTilesY;

        TileRenderer(
                const Spritesheet& spritesheet,
                const Texture& paletteTexture);

        void render(
                const Camera& camera,
                const std::vector<Tile>& tiles,
                int mapWidth,
                int mapHeight) const;

    private:
        SpriteRenderable renderable;

        const Texture& paletteTexture;

        bool TileRenderer::needsUpdate() const;

        void sendDataToGpu(
                const Camera& camera,
                const std::vector<Tile>& tiles,
                int mapWidth,
                int mapHeight) const;
    };

}  // namespace Rival

#endif  // TILE_RENDERER_H
