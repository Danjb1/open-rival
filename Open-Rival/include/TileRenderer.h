#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include <map>
#include <string>

#include "Unit.h"

namespace Rival {

    /**
     * Class responsible for rendering Tiles.
     *
     * It creates a Renderable for each Tile.
     */
    class TileRenderer {

    public:

        TileRenderer(
                std::map<int, Sprite>& tileSprites,
                Texture& paletteTexture);

        void render(
                std::vector<int>& tiles,
                int mapWidth,
                int mapHeight,
                bool wilderness);

    private:

        GLuint vao = 0;

        GLuint positionVbo = 0;

        GLuint texCoordVbo = 0;

        GLuint ibo = 0;

        const std::map<int, Sprite>& tileSprites;

        const Texture& paletteTexture;

        int getTileRenderPosX(int x);

        int getTileRenderPosY(int x, int y);

    };

}

#endif // TILE_RENDERER_H
