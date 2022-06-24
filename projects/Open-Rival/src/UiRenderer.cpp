#include "pch.h"

#include "UiRenderer.h"

#include <gl/glew.h>

#include "RenderUtils.h"
#include "Resources.h"

namespace Rival {

UiRenderer::UiRenderer(const Race& race, const TextureStore& textureStore)
    : textureStore(textureStore)
    , numUiImages(0)
    , mainUiRenderable(textureStore.getUiTextureAtlas(), maxUiImages)
    , minimapLeftBorder(GameInterface::minimapLeftBorder, textureStore.getUiTextureAtlas(), "img_ui_1060.tga")
    , minimapTopBorder(GameInterface::minimapTopBorder, textureStore.getUiTextureAtlas(), "img_ui_1058.tga")
    , minimapBottomBorder(GameInterface::minimapBottomBorder, textureStore.getUiTextureAtlas(), "img_ui_1059.tga")
    , mainPanel(
              GameInterface::mainPanel,
              textureStore.getUiTextureAtlas(),
              race == Race::Greenskin ? "img_ui_1123.tga" : "img_ui_1057.tga")
    , hideInventoryOverlay(
              GameInterface::hideInventoryOverlay,
              textureStore.getUiTextureAtlas(),
              race == Race::Greenskin ? "img_ui_1130.tga" : "img_ui_1064.tga")
    , statsPanel(
              GameInterface::statsPanel,
              textureStore.getUiTextureAtlas(),
              race == Race::Greenskin ? "img_ui_1136.tga" : "img_ui_1070.tga")
{
}

void UiRenderer::renderUi()
{
    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mainUiRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette().getId());

    // Bind vertex array
    glBindVertexArray(mainUiRenderable.getVao());

    // Update the data on the GPU
    if (needsUpdate())
    {
        sendDataToGpu();
    }

    // Render
    glDrawElements(
            mainUiRenderable.getDrawMode(),
            numUiImages * mainUiRenderable.getIndicesPerSprite(),
            GL_UNSIGNED_INT,
            nullptr);
}

bool UiRenderer::needsUpdate() const
{
    // Later, perhaps we can optimise this
    return true;
}

void UiRenderer::sendDataToGpu()
{
    // Determine the number of images to render
    bool inventoryVisible = isInventoryVisible();
    numUiImages = inventoryVisible ? maxUiImages - 1 : maxUiImages;

    // Create buffers to hold all our vertex data
    int numVertices = numUiImages * AtlasRenderable::numVerticesPerSprite;
    int positionDataSize = numVertices * AtlasRenderable::numVertexDimensions;
    int texCoordDataSize = numVertices * AtlasRenderable::numTexCoordDimensions;
    std::vector<GLfloat> positions;
    std::vector<GLfloat> texCoords;
    positions.reserve(positionDataSize);
    texCoords.reserve(texCoordDataSize);

    // Add data to our buffers
    minimapLeftBorder.addToBuffers(positions, texCoords);
    minimapTopBorder.addToBuffers(positions, texCoords);
    minimapBottomBorder.addToBuffers(positions, texCoords);
    mainPanel.addToBuffers(positions, texCoords);
    if (!inventoryVisible)
    {
        hideInventoryOverlay.addToBuffers(positions, texCoords);
    }
    statsPanel.addToBuffers(positions, texCoords);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, mainUiRenderable.getPositionVbo());
    int positionBufferSize = positions.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, mainUiRenderable.getTexCoordVbo());
    int texCoordBufferSize = texCoords.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());
}

bool UiRenderer::isInventoryVisible() const
{
    // TODO
    return false;
}

}  // namespace Rival
