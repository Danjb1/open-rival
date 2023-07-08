#pragma once

#include "gfx/GlewWrapper.h"

#include <memory>
#include <vector>

#include "game/Race.h"
#include "gfx/RenderUtils.h"
#include "gfx/renderable/SpriteRenderable.h"
#include "gfx/Spritesheet.h"

namespace Rival {

class Texture;

class MapBorderRenderer
{

public:
    MapBorderRenderer(
            Race race,
            int mapWidth,
            int mapHeight,
            const Spritesheet& spritesheet,
            std::shared_ptr<const Texture> paletteTexture);

    void
    createLeftEdge(std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords, int raceOffset, int mapHeight);

    void createTopEdge(std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords, int raceOffset, int mapWidth);

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
            std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords, int txIndex, float tileX, float tileY);

    void render() const;

private:
    static constexpr int txIndexTop = 0;
    static constexpr int txIndexRight = 1;
    static constexpr int txIndexBottom = 2;
    static constexpr int txIndexLeft = 3;
    static constexpr int txIndexTopLeft = 4;
    static constexpr int txIndexTopRight = 5;
    static constexpr int txIndexBottomRight = 6;
    static constexpr int txIndexBottomLeft = 7;

    std::shared_ptr<const Texture> paletteTexture;

    // The maximum number of border segments we can render
    int maxSegmentsToRender;

    SpriteRenderable renderable;

    int numSegments = 0;
};

}  // namespace Rival
