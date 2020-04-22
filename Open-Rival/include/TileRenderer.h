#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include <glm/mat4x4.hpp>
#include <map>
#include <string>

#include "Spritesheet.h"
#include "SpriteRenderable.h"
#include "Tile.h"
#include "Unit.h"

namespace Rival {

    /**
     * Class responsible for rendering Tiles.
     *
     * It renders all Tiles in a single draw operation.
     */
    class TileRenderer {

    public:

        // This is the max map size in the original game
        static const int maxTilesToRender = 210 * 134;

        TileRenderer(
                const Spritesheet& spritesheet,
                const Texture& paletteTexture);

        void render(
                glm::mat4 viewProjMatrix,
                std::vector<Tile>& tiles,
                int mapWidth,
                int mapHeight);

    private:

        SpriteRenderable renderable;

        const Texture& paletteTexture;

    };

}

#endif // TILE_RENDERER_H
