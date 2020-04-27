#ifndef MAP_BORDER_RENDERER_H
#define MAP_BORDER_RENDERER_H

#include <gl/glew.h>
#include <vector>

#include "Camera.h"
#include "Race.h"
#include "RenderUtils.h"
#include "Spritesheet.h"
#include "SpriteRenderable.h"
#include "Texture.h"

namespace Rival {

    class MapBorderRenderer {

    public:

        // The maximum number of border segments we can render
        static const int maxSegmentsToRender =
                (2 * RenderUtils::maxTilesX)
                + (2 * RenderUtils::maxTilesY);

        MapBorderRenderer(
                Race race,
                int mapWidth,
                int mapHeight,
                const Spritesheet& spritesheet,
                const Texture& paletteTexture);

        void MapBorderRenderer::createLeftEdge(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapHeight);

        void MapBorderRenderer::createTopEdge(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapWidth);

        void MapBorderRenderer::createRightEdge(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapWidth,
                int mapHeight);

        void MapBorderRenderer::createBottomEdge(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapWidth,
                int mapHeight);

        void MapBorderRenderer::createCorners(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapWidth,
                int mapHeight);

        void addDataToBuffers(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int txIndex,
                float tileX,
                float tileY);

        void render();

    private:

        SpriteRenderable renderable;

        const Texture& paletteTexture;

        int numSegments = 0;

    };

}

#endif // MAP_BORDER_RENDERER_H
