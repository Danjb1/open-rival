#include "ui/MenuRenderer.h"

#include "gfx/GlewWrapper.h"

#include "application/Window.h"
#include "gfx/GLUtils.h"
#include "gfx/Palette.h"
#include "gfx/RenderUtils.h"
#include "gfx/Shaders.h"
#include "gfx/renderable/TextureRenderable.h"
#include "utils/Rect.h"

namespace Rival {

const Rect backgroundImageRect(0, 0, 800, 600);

MenuRenderer::MenuRenderer(const TextureStore& textureStore, const Window* window, const Rect& viewport)
    : textureStore(textureStore)
    , window(window)
    , viewport(viewport)
    , cursorRenderer(textureStore, window)
    , backgroundImage(backgroundImageRect)
    , backgroundRenderable(textureStore.getMenuBackgroundTexture())
{
}

void MenuRenderer::render(int delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use indexed texture shader
    glUseProgram(Shaders::indexedTextureShader.programId);

    // Use the menu palette
    float paletteTxY = PaletteUtils::getPaletteTxY(PaletteUtils::paletteIndexMenu);
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, paletteTxY);

    renderBackground();
    renderCursor(delta);

    // Restore the default palette
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, 0);
}

// TMP: For now we render the background explicitly, but later we should render menus in a more generic way
void MenuRenderer::renderBackground()
{
    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backgroundRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette()->getId());

    // Bind vertex array
    glBindVertexArray(backgroundRenderable.getVao());

    // Update the data on the GPU
    // TODO: we only need to do this once
    sendBackgroundDataToGpu();

    // Render
    glDrawElements(GL_TRIANGLES, GLUtils::numIndicesForTriangles, GL_UNSIGNED_INT, nullptr);
}

void MenuRenderer::sendBackgroundDataToGpu()
{
    // Create buffers to hold all our vertex data
    int numVertices = TextureRenderable::numVertices;
    int positionDataSize = numVertices * TextureRenderable::numVertexDimensions;
    int texCoordDataSize = numVertices * TextureRenderable::numTexCoordDimensions;
    std::vector<GLfloat> positions;
    std::vector<GLfloat> texCoords;
    positions.reserve(positionDataSize);
    texCoords.reserve(texCoordDataSize);

    // Add data to our buffers
    backgroundImage.addToBuffers(positions, texCoords);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, backgroundRenderable.getPositionVbo());
    int positionBufferSize = static_cast<int>(positions.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, backgroundRenderable.getTexCoordVbo());
    int texCoordBufferSize = static_cast<int>(texCoords.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());
}

void MenuRenderer::renderCursor(int delta)
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

    // TMP: Use the game palette since we don't have menu cursor images yet
    float paletteTxY = PaletteUtils::getPaletteTxY(PaletteUtils::paletteIndexGame);
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, paletteTxY);

    // Render the cursor to the screen
    glViewport(0, 0, window->getWidth(), window->getHeight());
    cursorRenderer.render(Cursor::arrow, delta);
}

}  // namespace Rival
