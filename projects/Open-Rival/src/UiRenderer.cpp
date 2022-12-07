#include "pch.h"

#include "UiRenderer.h"

#include <gl/glew.h>

#include "InventoryComponent.h"
#include "MousePicker.h"
#include "PortraitComponent.h"
#include "RenderUtils.h"
#include "Resources.h"
#include "UnitPropsComponent.h"

namespace Rival {

UiRenderer::UiRenderer(
        const Race& race, const TextureStore& textureStore, const FontStore& fontStore, const Window& window)
    : textureStore(textureStore)
    , mainUiRenderable(textureStore.getUiTextureAtlas(), maxMainUiImages)
    , portraitRenderable(textureStore.getPortraitSpritesheet(), 1)
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
    , portrait(GameInterface::portrait, textureStore.getPortraitSpritesheet(), 0)
    , nameProperties({ &fontStore.getFontSmall() })
    , nameRenderable(
              Unit::maxNameLength, nameProperties, GameInterface::selectionName.x, GameInterface::selectionName.y)
    , textRenderer(window)
    , statsPanel(
              GameInterface::statsPanel,
              textureStore.getUiTextureAtlas(),
              race == Race::Greenskin ? "img_ui_1136.tga" : "img_ui_1070.tga")
{
}

void UiRenderer::renderUi(const Selection& selection)
{
    renderMainUi(selection);
    renderPortrait(selection);
    renderText(selection);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main UI
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UiRenderer::renderMainUi(const Selection& selection)
{
    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mainUiRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette().getId());

    // Bind vertex array
    glBindVertexArray(mainUiRenderable.getVao());

    // Update the data on the GPU
    if (mainUiNeedsUpdate())
    {
        sendMainUiDataToGpu(selection);
    }

    // Render
    glDrawElements(
            mainUiRenderable.getDrawMode(),
            numMainUiImages * mainUiRenderable.getIndicesPerSprite(),
            GL_UNSIGNED_INT,
            nullptr);
}

bool UiRenderer::mainUiNeedsUpdate() const
{
    // Later, perhaps we can optimise this
    return true;
}

void UiRenderer::sendMainUiDataToGpu(const Selection& selection)
{
    // First we need to know the number of images to render
    numMainUiImages = defaultNumMainUiImages;

    // Determine if the inventory should be rendered
    bool inventoryVisible = isInventoryVisible(selection);
    if (inventoryVisible)
    {
        // It's counter-intuitive, but to show the inventory we *don't* draw the overlay
        --numMainUiImages;
    }

    // Create buffers to hold all our vertex data
    int numVertices = numMainUiImages * AtlasRenderable::numVerticesPerSprite;
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

bool UiRenderer::isInventoryVisible(const Selection& selection) const
{
    auto selectedEntity = selection.weakSelectedEntity.lock();
    if (!selectedEntity)
    {
        return false;
    }

    auto inventory = selectedEntity->getComponent<InventoryComponent>(InventoryComponent::key);

    return inventory != nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Portrait
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UiRenderer::renderPortrait(const Selection& selection)
{
    int portraitId = -1;
    bool portraitVisible = isPortraitVisible(selection, portraitId);
    if (!portraitVisible)
    {
        return;
    }

    portrait.setSpriteIndex(portraitId);

    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, portraitRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette().getId());

    // Bind vertex array
    glBindVertexArray(portraitRenderable.getVao());

    // Update the data on the GPU
    if (portraitNeedsUpdate())
    {
        sendPortraitDataToGpu();
    }

    // Render
    glDrawElements(
            portraitRenderable.getDrawMode(), portraitRenderable.getIndicesPerSprite(), GL_UNSIGNED_INT, nullptr);
}

bool UiRenderer::portraitNeedsUpdate() const
{
    // Later, perhaps we can optimise this
    return true;
}

void UiRenderer::sendPortraitDataToGpu()
{
    // Create buffers to hold all our vertex data
    int numVertices = SpriteRenderable::numVerticesPerSprite;
    int positionDataSize = numVertices * SpriteRenderable::numVertexDimensions;
    int texCoordDataSize = numVertices * SpriteRenderable::numTexCoordDimensions;
    std::vector<GLfloat> positions;
    std::vector<GLfloat> texCoords;
    positions.reserve(positionDataSize);
    texCoords.reserve(texCoordDataSize);

    // Add data to our buffers
    portrait.addToBuffers(positions, texCoords);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, portraitRenderable.getPositionVbo());
    int positionBufferSize = positions.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, portraitRenderable.getTexCoordVbo());
    int texCoordBufferSize = texCoords.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());
}

bool UiRenderer::isPortraitVisible(const Selection& selection, int& outPortraitId) const
{
    auto selectedEntity = selection.weakSelectedEntity.lock();
    if (!selectedEntity)
    {
        return false;
    }

    auto portraitComp = selectedEntity->getComponent<PortraitComponent>(PortraitComponent::key);
    if (!portraitComp)
    {
        return false;
    }

    outPortraitId = portraitComp->getPortraitId();
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Text
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UiRenderer::renderText(const Selection& selection)
{
    textRenderables.clear();

    std::string name;
    if (isNameVisible(selection, name))
    {
        nameRenderable.setTextSpan({ name, TextRenderable::defaultColor });
        textRenderables.push_back(&nameRenderable);
    }

    textRenderer.render(textRenderables);
}

bool UiRenderer::isNameVisible(const Selection& selection, std::string& outName) const
{
    auto selectedEntity = selection.weakSelectedEntity.lock();
    if (!selectedEntity)
    {
        return false;
    }

    auto unitProps = selectedEntity->getComponent<UnitPropsComponent>(UnitPropsComponent::key);
    if (!unitProps)
    {
        return false;
    }

    outName = unitProps->getName();
    return true;
}

}  // namespace Rival
