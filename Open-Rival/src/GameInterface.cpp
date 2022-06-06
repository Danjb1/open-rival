#include "pch.h"

#include "GameInterface.h"

#include "RenderUtils.h"

namespace Rival { namespace GameInterface {

    ///////////////////////////////////////////////////////////////////////////
    // UI elements
    ///////////////////////////////////////////////////////////////////////////

    const Rect minimapLeftBorder(0, uiPosY, 5, uiHeight);

    const Rect minimapTopBorder(static_cast<int>(minimapLeftBorder.x + minimapLeftBorder.width), uiPosY, 210, 5);

    const Rect minimapBottomBorder(0, uiPosY + 205, 210, 5);

    const Rect mainPanel(static_cast<int>(minimapTopBorder.x + minimapTopBorder.width), uiPosY, 425, uiHeight);

    const Rect inventoryOverlay(static_cast<int>(mainPanel.x) + 30, static_cast<int>(mainPanel.y) + 71, 211, 47);

    const Rect statsPanel(static_cast<int>(mainPanel.x + mainPanel.width), uiPosY, 160, uiHeight);

    ///////////////////////////////////////////////////////////////////////////
    // UiImage class
    ///////////////////////////////////////////////////////////////////////////

    UiImage::UiImage(Rect pos, const TextureAtlas& texAtlas, const std::string imageKey)
        : pos(pos)
        , texAtlas(texAtlas)
        , imageKey(imageKey)
    {
    }

    void UiImage::addToBuffers(std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords) const
    {

        // Define vertex positions
        float x1 = pos.x;
        float y1 = pos.y;
        float x2 = x1 + pos.width;
        float y2 = y1 + pos.height;
        float z = 0;
        std::vector<GLfloat> thisVertexData = { x1, y1, z, x2, y1, z, x2, y2, z, x1, y2, z };

        // Determine texture co-ordinates
        std::vector<GLfloat> thisTexCoords = texAtlas.getTexCoords(imageKey);

        // Copy this data into the main buffers
        positions.insert(positions.cend(), thisVertexData.cbegin(), thisVertexData.cend());
        texCoords.insert(texCoords.cend(), thisTexCoords.cbegin(), thisTexCoords.cend());
    }

}}  // namespace Rival::GameInterface
