#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include <glm/mat4x4.hpp>
#include <map>
#include <string>

#include "Renderable.h"
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

        TileRenderer(
                std::map<int, Sprite>& tileSprites,
                Texture& paletteTexture);

        void render(
                glm::mat4 viewProjMatrix,
                std::vector<Tile>& tiles,
                int mapWidth,
                int mapHeight,
                bool wilderness);

    private:

        Renderable renderable;

        const std::map<int, Sprite>& tileSprites;

        const Texture& paletteTexture;

        int getTileRenderPosX(int x);

        int getTileRenderPosY(int x, int y);

    };

}

#endif // TILE_RENDERER_H
