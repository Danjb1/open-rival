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

        void render(std::vector<int>& tiles);

    private:

        const std::map<int, Sprite>& tileSprites;

        const Texture& paletteTexture;

        float getTileRenderPosX(int x);

        float getTileRenderPosY(int y);

    };

}

#endif // TILE_RENDERER_H
