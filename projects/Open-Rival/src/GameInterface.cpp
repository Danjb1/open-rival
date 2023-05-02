#include "pch.h"

#include "GameInterface.h"

#include "RenderUtils.h"

namespace Rival { namespace GameInterface {

///////////////////////////////////////////////////////////////////////////
// UI elements
///////////////////////////////////////////////////////////////////////////

// Cursor
const Rect cursor(0, 0, RenderUtils::cursorWidthPx, RenderUtils::cursorHeightPx);

// Minimap
const Rect minimapLeftBorder(0, uiPosY, 5, uiHeight);
const Rect minimapTopBorder(static_cast<int>(minimapLeftBorder.x + minimapLeftBorder.width), uiPosY, 210, 5);
const Rect minimapBottomBorder(0, uiPosY + 205, 210, 5);

// Main panel
const Rect mainPanel(static_cast<int>(minimapTopBorder.x + minimapTopBorder.width), uiPosY, 425, uiHeight);
const Rect hideInventoryOverlay(static_cast<int>(mainPanel.x) + 31, static_cast<int>(mainPanel.y) + 74, 211, 47);
const Rect portrait(static_cast<int>(mainPanel.x) + 240, static_cast<int>(mainPanel.y) + 3, 42, 45);
const glm::vec2 selectionName(static_cast<int>(portrait.x) + 42, static_cast<int>(portrait.y) + 12);

// Stats panel
const Rect statsPanel(static_cast<int>(mainPanel.x + mainPanel.width), uiPosY, 160, uiHeight);

///////////////////////////////////////////////////////////////////////////
// UiImage class
///////////////////////////////////////////////////////////////////////////

UiImage::UiImage(Rect pos, std::shared_ptr<const TextureAtlas> texAtlas, const std::string imageKey)
    : pos(pos)
    , texAtlas(texAtlas)
    , imageKey(imageKey)
{
}

UiImage::UiImage(Rect pos, const Spritesheet& spritesheet, int spriteIndex)
    : pos(pos)
    , spritesheet(&spritesheet)
    , spriteIndex(spriteIndex)
{
}

UiImage::UiImage(Rect pos)
    : pos(pos)
{
}

void UiImage::setSpriteIndex(int newSpriteIndex)
{
    spriteIndex = newSpriteIndex;
}

void UiImage::addToBuffers(std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords) const
{
    // Define vertex positions
    float x1 = pos.x;
    float y1 = pos.y;
    float x2 = x1 + pos.width;
    float y2 = y1 + pos.height;
    float z = 0;
    std::vector<GLfloat> thisVertexData = {
        x1, y1, z,  //
        x2, y1, z,  //
        x2, y2, z,  //
        x1, y2, z   //
    };

    // Determine texture co-ordinates
    std::vector<GLfloat> thisTexCoords;
    if (texAtlas)
    {
        thisTexCoords = texAtlas->getTexCoords(imageKey);
    }
    else if (spritesheet)
    {
        thisTexCoords = spritesheet->getTexCoords(spriteIndex);
    }
    else
    {
        const float tx1 = 0.f;
        const float tx2 = 1.f;
        const float ty1 = 0.f;
        const float ty2 = 1.f;

        thisTexCoords = {
            tx1, ty1,  //
            tx2, ty1,  //
            tx2, ty2,  //
            tx1, ty2   //
        };
    }

    // Copy this data into the main buffers
    positions.insert(positions.cend(), thisVertexData.cbegin(), thisVertexData.cend());
    texCoords.insert(texCoords.cend(), thisTexCoords.cbegin(), thisTexCoords.cend());
}

}}  // namespace Rival::GameInterface
