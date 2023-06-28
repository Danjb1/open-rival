#include "ui/CursorRenderer.h"

#include "SDLWrapper.h"
#define GLEW_STATIC
#include <gl/glew.h>

#include "Resources.h"

namespace Rival {

CursorRenderer::CursorRenderer(const TextureStore& textureStore, const Window* window)
    : textureStore(textureStore)
    , window(window)
    , cursorRenderable(textureStore.getCursorSpritesheet(), 1)
    , cursorImage(GameInterface::cursor, textureStore.getCursorSpritesheet(), 0)
{
}

void CursorRenderer::render(const CursorDef& cursorDef, int delta)
{
    // Get the mouse position relative to the window, in pixels
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Calculate the mouse position in the range 0-1.
    // (If the mouse is outside of the window, these values will lie outside the range).
    float normalizedMouseX = static_cast<float>(mouseX) / window->getWidth();
    float normalizedMouseY = static_cast<float>(mouseY) / window->getHeight();

    // Set the cursor image position
    cursorImage.pos.x = normalizedMouseX * RenderUtils::getMenuWidth(window->getAspectRatio());
    cursorImage.pos.y = normalizedMouseY * RenderUtils::menuHeight;

    // Offset the image based on the cursor hotspot
    cursorImage.pos.x -= cursorDef.hotspotX * RenderUtils::cursorWidthPx;
    cursorImage.pos.y -= cursorDef.hotspotY * RenderUtils::cursorHeightPx;

    // Animate
    int spriteIndex = animateCursor(cursorDef, delta);
    cursorImage.setSpriteIndex(spriteIndex);

    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cursorRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette()->getId());

    // Bind vertex array
    glBindVertexArray(cursorRenderable.getVao());

    // Create buffers to hold all our vertex data
    int numVertices = SpriteRenderable::numVerticesPerSprite;
    int positionDataSize = numVertices * SpriteRenderable::numVertexDimensions;
    int texCoordDataSize = numVertices * SpriteRenderable::numTexCoordDimensions;
    std::vector<GLfloat> positions;
    std::vector<GLfloat> texCoords;
    positions.reserve(positionDataSize);
    texCoords.reserve(texCoordDataSize);

    // Add data to our buffers
    cursorImage.addToBuffers(positions, texCoords);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, cursorRenderable.getPositionVbo());
    int positionBufferSize = positions.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, cursorRenderable.getTexCoordVbo());
    int texCoordBufferSize = texCoords.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());

    // Render
    glDrawElements(cursorRenderable.getDrawMode(), cursorRenderable.getIndicesPerSprite(), GL_UNSIGNED_INT, nullptr);
}

int CursorRenderer::animateCursor(const CursorDef& cursorDef, int delta)
{
    if (cursorDef == prevCursor)
    {
        // Cursor has not changed since the last frame
        if (cursorDef.startIndex == cursorDef.endIndex)
        {
            // Static cursor
            return cursorDef.startIndex;
        }
        else
        {
            // Animated cursor
            cursorAnimTime += delta;

            int numFrames = cursorDef.endIndex - cursorDef.startIndex;
            int totalAnimTime = msPerCursorFrame * numFrames;
            int relativeAnimTime = cursorAnimTime % totalAnimTime;
            int relativeIndex = relativeAnimTime / msPerCursorFrame;

            return cursorDef.startIndex + relativeIndex;
        }
    }

    // Cursor has changed
    prevCursor = cursorDef;
    cursorAnimTime = 0;
    return cursorDef.startIndex;
}

}  // namespace Rival
