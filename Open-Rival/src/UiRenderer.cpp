#include "pch.h"
#include "UiRenderer.h"

#include <gl/glew.h>

#include "RenderUtils.h"

namespace Rival {

    UiRenderer::UiRenderer(const Resources& res)
        : res(res),
          mainPanel(res.getUiTextureAtlas(), 1) {}

    void UiRenderer::renderUi(int y) const {

        // Use textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mainPanel.getTextureId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, res.getPalette().getId());

        // Bind vertex array
        glBindVertexArray(mainPanel.getVao());

        // Update the data on the GPU
        if (needsUpdate()) {
            sendDataToGpu(y);
        }

        // Render
        glDrawElements(
                mainPanel.getDrawMode(),
                mainPanel.getIndicesPerSprite(),
                GL_UNSIGNED_INT,
                nullptr);
    }

    bool UiRenderer::needsUpdate() const {
        // Later, perhaps we can optimise this
        return true;
    }

    void UiRenderer::sendDataToGpu(int y) const {

        const TextureAtlas& uiTexAtlas = res.getUiTextureAtlas();

        // Define vertex positions
        float x1 = -1;
        float y1 = -1 + (y / 600.0f);
        float x2 = 1;   //x1 + static_cast<float>(uiTexAtlas.getImageWidth("img_ui_019E79C3.tga"));
        float y2 = -1;  //y1 - static_cast<float>(uiTexAtlas.getImageHeight("img_ui_019E79C3.tga"));
        float z = 0;
        std::vector<GLfloat> vertexData = {
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
        };

        // Determine texture co-ordinates
        std::vector<GLfloat> texCoords = uiTexAtlas.getTexCoords("img_ui_019E79C3.tga");

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, mainPanel.getPositionVbo());
        int positionBufferSize =
                vertexData.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                positionBufferSize,
                vertexData.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, mainPanel.getTexCoordVbo());
        int texCoordBufferSize =
                texCoords.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                texCoordBufferSize,
                texCoords.data());
    }

}  // namespace Rival
