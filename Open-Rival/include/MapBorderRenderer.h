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
        MapBorderRenderer(
                Race race,
                int mapWidth,
                int mapHeight,
                const Spritesheet& spritesheet,
                const Texture& paletteTexture);

        void createLeftEdge(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int raceOffset,
                int mapHeight);

        void createTopEdge(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int raceOffset,
                int mapWidth);

        void createRightEdge(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int raceOffset,
                int mapWidth,
                int mapHeight);

        void createBottomEdge(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int raceOffset,
                int mapWidth,
                int mapHeight);

        void createCorners(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int raceOffset,
                int mapWidth,
                int mapHeight);

        void addDataToBuffers(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords,
                int txIndex,
                float tileX,
                float tileY);

        void render() const;

    private:
        static const int txIndexTop = 0;
        static const int txIndexRight = 1;
        static const int txIndexBottom = 2;
        static const int txIndexLeft = 3;
        static const int txIndexTopLeft = 4;
        static const int txIndexTopRight = 5;
        static const int txIndexBottomRight = 6;
        static const int txIndexBottomLeft = 7;

        const Texture& paletteTexture;

        // The maximum number of border segments we can render
        int maxSegmentsToRender;

        SpriteRenderable renderable;

        int numSegments = 0;
    };

}  // namespace Rival

#endif  // MAP_BORDER_RENDERER_H
