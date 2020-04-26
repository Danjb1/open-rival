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

        // The maximum number of border segments we can render.
        // Each border segment spans 2 tiles horizontally, or 1 tile
        // vertically.
        static const int maxSegmentsToRender =
                (RenderUtils::maxTilesX / 2)    // top edge
                + (RenderUtils::maxTilesX / 2)  // bottom edge
                + RenderUtils::maxTilesY        // left edge
                + RenderUtils::maxTilesY;       // right edge

        MapBorderRenderer(
                const Spritesheet& spritesheet,
                const Texture& paletteTexture);

        void render(
                const Race race,
                const Camera& camera,
                int mapWidth,
                int mapHeight);

        void MapBorderRenderer::createLeftEdge(
                int& renderedSegments,
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapHeight);

        void MapBorderRenderer::createTopEdge(
                int& renderedSegments,
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapWidth);

        void MapBorderRenderer::createRightEdge(
                int& renderedSegments,
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapWidth,
                int mapHeight);

        void MapBorderRenderer::createBottomEdge(
                int& renderedSegments,
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int mapWidth,
                int mapHeight);

        void addDataToBuffers(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int txIndex,
                int tileX,
                int tileY);

    private:

        SpriteRenderable renderable;

        const Texture& paletteTexture;

    };

}

#endif // MAP_BORDER_RENDERER_H
