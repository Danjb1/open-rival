#include "gfx/renderer/MenuTextRenderer.h"

#include <glm/glm.hpp>

#include "application/Window.h"
#include "gfx/RenderUtils.h"
#include "gfx/Shaders.h"

namespace Rival {

MenuTextRenderer::MenuTextRenderer(const Window* window)
    : window(window)
    , textRenderer()
{
}

void MenuTextRenderer::render(const std::vector<const TextRenderable*> textRenderables) const
{
    // Disable depth testing for now (we may use it later)
    glDisable(GL_DEPTH_TEST);

    // Use font shader
    glUseProgram(Shaders::fontShader.programId);

    // Project to menu
    glm::mat4 viewProjMatrix = RenderUtils::createMenuViewProjectionMatrix(window->getAspectRatio());

    // Set uniform values
    glUniformMatrix4fv(Shaders::fontShader.viewProjMatrixUniformLoc, 1, GL_FALSE, &viewProjMatrix[0][0]);
    glUniform1i(Shaders::fontShader.texUnitUniformLoc, 0);

    // Render!
    for (const TextRenderable* textRenderable : textRenderables)
    {
        textRenderer.render(*textRenderable);
    }
}

}  // namespace Rival
