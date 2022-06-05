#include "pch.h"
#include "MenuTextRenderer.h"

#include <glm/glm.hpp>

#include "RenderUtils.h"
#include "Shaders.h"

namespace Rival {

    MenuTextRenderer::MenuTextRenderer(const Window& window) : window(&window), textRenderer() {}

    void MenuTextRenderer::render(const std::vector<TextRenderable*> textRenderables) const
    {
        // Disable depth testing for now (we may use it later)
        glDisable(GL_DEPTH_TEST);

        // Use font shader
        glUseProgram(Shaders::fontShader.programId);

        // Project to menu
        glm::mat4 viewProjMatrix = RenderUtils::createMenuProjection(window->getAspectRatio());

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
