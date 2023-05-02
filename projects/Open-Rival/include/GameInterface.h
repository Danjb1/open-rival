#pragma once

#include <gl/glew.h>
#include <glm/vec2.hpp>

#include <string>
#include <vector>

#include "Rect.h"
#include "RenderUtils.h"
#include "Spritesheet.h"
#include "TextureAtlas.h"

namespace Rival { namespace GameInterface {

/**
 * Height of the in-game UI, in menu co-ordinates.
 */
static constexpr int uiHeight = 144;

/**
 * Y-position of the in-game UI, in menu co-ordinates.
 */
static constexpr int uiPosY = RenderUtils::menuHeight - uiHeight;

// UI element positions, in menu co-ordinates
extern const Rect cursor;
extern const Rect minimapLeftBorder;
extern const Rect minimapTopBorder;
extern const Rect minimapBottomBorder;
extern const Rect mainPanel;
extern const Rect hideInventoryOverlay;
extern const Rect portrait;
extern const glm::vec2 selectionName;
extern const Rect statsPanel;

/**
 * A UI image with a position, that can be used to populate vertex position and texture co-ordinate buffers.
 */
class UiImage
{
public:
    Rect pos;

    /** Constructor for a UiImage that uses a TextureAtlas. */
    UiImage(Rect pos, std::shared_ptr<const TextureAtlas> texAtlas, const std::string imageKey);

    /** Constructor for a UiImage that uses a Spritesheet. */
    UiImage(Rect pos, const Spritesheet& spritesheet, int spriteIndex);

    /** Constructor for a UiImage that renders an entire texture. */
    UiImage(Rect pos);

    void setSpriteIndex(int newSpriteIndex);

    void addToBuffers(std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords) const;

private:
    // TextureAtlas version
    std::shared_ptr<const TextureAtlas> texAtlas;
    const std::string imageKey;

    // Spritesheet version
    const Spritesheet* spritesheet = nullptr;
    int spriteIndex = 0;
};

}}  // namespace Rival::GameInterface
