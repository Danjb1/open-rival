#include "gfx/opengl/renderer/MenuRenderer.h"

#include "gfx/opengl/GLWrapper.h"

#include "gfx/Palette.h"
#include "gfx/PaletteUtils.h"
#include "gfx/RenderUtils.h"
#include "gfx/opengl/GLUtils.h"
#include "gfx/opengl/Shaders.h"
#include "gfx/opengl/renderable/TextureRenderable.h"
#include "gfx/opengl/renderer/UiRenderer.h"
#include "utils/Rect.h"
#include "window/Window.h"

namespace Rival {

const Rect backgroundImageRect(0, 0, 800, 600);

MenuRenderer::MenuRenderer(const TextureStore& textureStore, const Window* window, const Rect& viewport)
    : textureStore(textureStore)
    , window(window)
    , viewport(viewport)
    , backgroundImage(backgroundImageRect)
    , backgroundRenderable(textureStore.getMenuBackgroundTexture())
{
}

void MenuRenderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use indexed texture shader
    glUseProgram(Shaders::indexedTextureShader.programId);

    // Use the menu palette
    float paletteTxY = PaletteUtils::getPaletteTxY(PaletteUtils::paletteIndexMenu);
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, paletteTxY);

    renderBackground();

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
    UiRenderer::addToBuffers(backgroundImage, positions, texCoords);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, backgroundRenderable.getPositionVbo());
    int positionBufferSize = static_cast<int>(positions.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, backgroundRenderable.getTexCoordVbo());
    int texCoordBufferSize = static_cast<int>(texCoords.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());
}

}  // namespace Rival
