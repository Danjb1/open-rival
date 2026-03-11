#include "game/GameInterface.h"

#include "gfx/RenderUtils.h"

namespace Rival { namespace GameInterface {

///////////////////////////////////////////////////////////////////////////
// UI elements
///////////////////////////////////////////////////////////////////////////

// Cursor
const Rect cursor(0, 0, RenderUtils::cursorWidthPx, RenderUtils::cursorHeightPx);

// Minimap
const Rect minimapLeftBorder(0, uiPosY, 5, uiHeight);
const Rect minimapTopBorder(static_cast<int>(minimapLeftBorder.x + minimapLeftBorder.width), uiPosY, 210, 5);
const Rect minimapBottomBorder(5, uiPosY + uiHeight - 5, 210, 5);

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

UiImage::UiImage(Rect pos, std::shared_ptr<const Spritesheet> spritesheet, int spriteIndex)
    : pos(pos)
    , spritesheet(spritesheet)
    , spriteIndex(spriteIndex)
{
}

UiImage::UiImage(Rect pos)
    : pos(pos)
{
}

Rect UiImage::getPos() const
{
    return pos;
}

void UiImage::setPos(const Rect& newPos)
{
    pos = newPos;
}

void UiImage::setSpriteIndex(int newSpriteIndex)
{
    spriteIndex = newSpriteIndex;
}

}}  // namespace Rival::GameInterface
