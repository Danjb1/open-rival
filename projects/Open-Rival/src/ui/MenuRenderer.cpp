#include "pch.h"

#include "ui/MenuRenderer.h"

#include <gl/glew.h>

#include "Rect.h"
#include "RenderUtils.h"
#include "Resources.h"
#include "Shaders.h"
#include "Window.h"

namespace Rival {

MenuRenderer::MenuRenderer(const Window* window, const Rect& viewport, const Resources& res)
    : window(window)
    , viewport(viewport)
    , cursorRenderer(res, window)
{
}

void MenuRenderer::render(int delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use indexed texture shader
    glUseProgram(Shaders::indexedTextureShader.programId);

    // Use the menu palette
    // TODO: The cursor colors seem incorrect, but I am not sure if that is a problem with the way our menu palette is
    // defined, or whether we need to introduce a new palette for menu cursors.
    // Curiously, there are 3 identical cursor images that we are not currently using, which are probably meant to be
    // used in menus: img_ui_0144.tga, img_ui_0243.tga, img_ui_0359.tga.
    // However, these seem to use the same colors as the in-game cursor, so they don't help us here.
    float paletteTxY = PaletteUtils::getPaletteTxY(PaletteUtils::paletteIndexMenu);
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, paletteTxY);

    renderCursor(delta);

    // Restore the default palette
    glUniform1f(Shaders::indexedTextureShader.paletteTxYUnitUniformLoc, 0);
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

    // Render the cursor to the screen
    glViewport(0, 0, window->getWidth(), window->getHeight());
    cursorRenderer.render(Cursor::arrow, delta);
}

}  // namespace Rival
