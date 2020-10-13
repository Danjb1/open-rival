#include "pch.h"
#include "UiRenderer.h"

#include <gl/glew.h>

#include "RenderUtils.h"

namespace Rival {

    UiRenderer::UiRenderer(const Resources& res, const Window& window)
        : res(res),
          window(window),
          mainUiRenderable(res.getUiTextureAtlas(), maxUiImages) {}

    void UiRenderer::renderUi() {

        // Use textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mainUiRenderable.getTextureId());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, res.getPalette().getId());

        // Bind vertex array
        glBindVertexArray(mainUiRenderable.getVao());

        // Update the data on the GPU
        if (needsUpdate()) {
            sendDataToGpu();
        }

        // Render
        glDrawElements(
                mainUiRenderable.getDrawMode(),
                numUiImages * mainUiRenderable.getIndicesPerSprite(),
                GL_UNSIGNED_INT,
                nullptr);
    }

    bool UiRenderer::needsUpdate() const {
        // Later, perhaps we can optimise this
        return true;
    }

    void UiRenderer::sendDataToGpu() {

        // For now, we always draw the same number of images
        numUiImages = maxUiImages;

        // Create buffers to hold all our vertex data
        int numVertices = numUiImages * AtlasRenderable::numVerticesPerSprite;
        int positionDataSize = numVertices * AtlasRenderable::numVertexDimensions;
        int texCoordDataSize = numVertices * AtlasRenderable::numTexCoordDimensions;
        std::vector<GLfloat> positions;
        std::vector<GLfloat> texCoords;
        positions.reserve(positionDataSize);
        texCoords.reserve(texCoordDataSize);

        // Add data to our buffers
        addMainPanelToBuffers(positions, texCoords);
        addStatsPanelToBuffers(positions, texCoords);

        // Upload position data
        glBindBuffer(GL_ARRAY_BUFFER, mainUiRenderable.getPositionVbo());
        int positionBufferSize =
                positions.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                positionBufferSize,
                positions.data());

        // Upload tex co-ord data
        glBindBuffer(GL_ARRAY_BUFFER, mainUiRenderable.getTexCoordVbo());
        int texCoordBufferSize =
                texCoords.size() * sizeof(GLfloat);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                texCoordBufferSize,
                texCoords.data());
    }

    void UiRenderer::addMainPanelToBuffers(
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords) const {

        // Define vertex positions
        float x1 = RenderUtils::minimapWidth;
        float y1 = RenderUtils::menuHeight - RenderUtils::uiHeight;
        float x2 = x1 + RenderUtils::uiMainPanelWidth;
        float y2 = RenderUtils::menuHeight;
        float z = 0;
        std::vector<GLfloat> thisVertexData = {
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
        };

        // Determine texture co-ordinates
        const TextureAtlas& uiTexAtlas = res.getUiTextureAtlas();
        std::vector<GLfloat> thisTexCoords =
                uiTexAtlas.getTexCoords("img_ui_019E79C3.tga");

        // Copy this data into the main buffers
        positions.insert(
                positions.end(),
                thisVertexData.begin(),
                thisVertexData.end());
        texCoords.insert(
                texCoords.end(),
                thisTexCoords.begin(),
                thisTexCoords.end());
    }

    void UiRenderer::addStatsPanelToBuffers(
            std::vector<GLfloat>& positions,
            std::vector<GLfloat>& texCoords) const {

        // Define vertex positions
        float x1 = RenderUtils::minimapWidth + RenderUtils::uiMainPanelWidth;
        float y1 = RenderUtils::menuHeight - RenderUtils::uiHeight;
        float x2 = static_cast<float>(
                RenderUtils::getMenuWidth(window.getAspectRatio()));
        float y2 = RenderUtils::menuHeight;
        float z = 0;
        std::vector<GLfloat> thisVertexData = {
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
        };

        // Determine texture co-ordinates
        const TextureAtlas& uiTexAtlas = res.getUiTextureAtlas();
        std::vector<GLfloat> thisTexCoords =
                uiTexAtlas.getTexCoords("img_ui_019FD2F7.tga");

        // Copy this data into the main buffers
        positions.insert(
                positions.end(),
                thisVertexData.begin(),
                thisVertexData.end());
        texCoords.insert(
                texCoords.end(),
                thisTexCoords.begin(),
                thisTexCoords.end());
    }

}  // namespace Rival
