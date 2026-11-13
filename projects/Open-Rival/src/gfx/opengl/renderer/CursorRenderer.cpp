#include "gfx/opengl/renderer/CursorRenderer.h"

#include "gfx/opengl/GLWrapper.h"

#include <cmath>  // fmod

#include "application/Resources.h"
#include "gfx/Palette.h"
#include "gfx/PaletteUtils.h"
#include "gfx/RenderUtils.h"
#include "gfx/opengl/Shaders.h"
#include "gfx/opengl/renderer/UiRenderer.h"

namespace Rival {

CursorRenderer::CursorRenderer(const TextureStore& textureStore, const Window* window, const Input& input)
    : textureStore(textureStore)
    , window(window)
    , input(input)
    , cursorRenderable(textureStore.getCursorSpritesheet(), 1)
    , cursorImage(GameInterface::cursor, textureStore.getCursorSpritesheet(), 0)
{
}

void CursorRenderer::render(const CursorDef& cursorDef, float timeSinceLastRender)
{
    // Disable depth testing since the cursor is always on top
    glDisable(GL_DEPTH_TEST);

    // Use indexed texture shader
    glUseProgram(Shaders::indexedTextureShader.programId);

    // Determine our view-projection matrix
    glm::mat4 viewProjMatrix = RenderUtils::createMenuViewProjectionMatrix(window->getAspectRatio());

    // Set uniform values
    glUniformMatrix4fv(Shaders::indexedTextureShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::indexedTextureShader.texUnitUniformLoc, 0);
    glUniform1i(Shaders::indexedTextureShader.paletteTexUnitUniformLoc, 1);
    glUniform1f(Shaders::indexedTextureShader.transparentIndex, Palette::cursorTransparentColor);

    // Cursor images use the game palette
    float paletteTxY = PaletteUtils::getPaletteTxY(PaletteUtils::paletteIndexGame);
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, paletteTxY);

    // Render the cursor to the whole screen
    glViewport(0, 0, window->getWidth(), window->getHeight());

    // Get the mouse position relative to the window, in pixels
    const int mouseX = input.getMouseX();
    const int mouseY = input.getMouseY();

    // Calculate the mouse position in the range 0-1.
    // (If the mouse is outside of the window, these values will lie outside the range).
    float normalizedMouseX = static_cast<float>(mouseX) / window->getWidth();
    float normalizedMouseY = static_cast<float>(mouseY) / window->getHeight();

    // Determine the cursor image position
    Rect pos = cursorImage.getPos();
    pos.x = normalizedMouseX * RenderUtils::getMenuWidth(window->getAspectRatio());
    pos.y = normalizedMouseY * RenderUtils::menuHeight;

    // Offset the image based on the cursor hotspot
    pos.x -= cursorDef.hotspotX * RenderUtils::cursorWidthPx;
    pos.y -= cursorDef.hotspotY * RenderUtils::cursorHeightPx;
    cursorImage.setPos(pos);

    // Animate
    int spriteIndex = animateCursor(cursorDef, timeSinceLastRender);
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
    UiRenderer::addToBuffers(cursorImage, positions, texCoords);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, cursorRenderable.getPositionVbo());
    int positionBufferSize = static_cast<int>(positions.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, cursorRenderable.getTexCoordVbo());
    int texCoordBufferSize = static_cast<int>(texCoords.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());

    // Render
    glDrawElements(cursorRenderable.getDrawMode(), cursorRenderable.getIndicesPerSprite(), GL_UNSIGNED_INT, nullptr);
}

int CursorRenderer::animateCursor(const CursorDef& cursorDef, float timeSinceLastRender)
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
            cursorAnimTime += timeSinceLastRender;

            // endIndex is inclusive
            int numFrames = cursorDef.endIndex - cursorDef.startIndex + 1;
            float totalAnimTime = timePerCursorFrame * numFrames;
            float relativeAnimTime = std::fmod(cursorAnimTime, totalAnimTime);
            int relativeIndex = static_cast<int>(relativeAnimTime / timePerCursorFrame);

            return cursorDef.startIndex + relativeIndex;
        }
    }

    // Cursor has changed
    prevCursor = cursorDef;
    cursorAnimTime = 0;
    return cursorDef.startIndex;
}

}  // namespace Rival
