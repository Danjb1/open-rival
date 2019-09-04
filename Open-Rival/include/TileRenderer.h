#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include <glm/mat4x4.hpp>
#include <map>
#include <string>

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
