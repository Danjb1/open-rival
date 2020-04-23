#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

namespace Rival {
    namespace RenderUtils {

        // Size of a single Tile image, in pixels
        static const int tileSpriteWidthPx = 64;
        static const int tileSpriteHeightPx = 64;

        // Size of a single Tile image, ignoring empty space, in pixels
        static const int tileWidthPx = 64;
        static const int tileHeightPx = 32;

        // Size of a single Unit image, in pixels
        static const int unitWidthPx = 128;
        static const int unitHeightPx = 128;

        /**
         * Gets the x-position at which a tile with the given x co-ordinate
         * should be rendered, in pixels.
         *
         * Normally we would send our vertices to the GPU in world units, and
         * rely on the OpenGL projection to handle the conversion to pixels.
         * However, there is no way OpenGL can position our tiles for us due
         * to their strange isometric positioning, so we convert our vertices
         * to pixels manually instead.
         */
        int getRenderPosX(int x);

        /**
         * Gets the y-position at which a tile with the given x and y
         * co-ordinates should be rendered, in pixels.
         *
         * We have to consider both axes here because the y-position is
         * actually dependent on the x-position.
         *
         * See documentation for getRenderPosX, above.
         */
        int getRenderPosY(int x, int y);

    }
}

#endif // RENDER_UTILS_H
