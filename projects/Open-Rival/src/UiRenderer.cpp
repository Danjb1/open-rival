#include "pch.h"

#include "UiRenderer.h"

#include <gl/glew.h>

#include "InventoryComponent.h"
#include "PlayerContext.h"
#include "PlayerState.h"
#include "PortraitComponent.h"
#include "RenderUtils.h"
#include "Resources.h"
#include "UnitPropsComponent.h"

namespace Rival {

UiRenderer::UiRenderer(
        const PlayerStore& playerStore,
        const TextureStore& textureStore,
        const FontStore& fontStore,
        const Window& window,
        const PlayerContext& playerContext)
    : textureStore(textureStore)
    , window(window)
    , playerStore(playerStore)
    , playerContext(playerContext)

    // Main UI
    , mainUiRenderable(textureStore.getUiTextureAtlas(), maxMainUiImages)
    , minimapLeftBorder(GameInterface::minimapLeftBorder, textureStore.getUiTextureAtlas(), "img_ui_1060.tga")
    , minimapTopBorder(GameInterface::minimapTopBorder, textureStore.getUiTextureAtlas(), "img_ui_1058.tga")
    , minimapBottomBorder(GameInterface::minimapBottomBorder, textureStore.getUiTextureAtlas(), "img_ui_1059.tga")
    , mainPanel(
              GameInterface::mainPanel,
              textureStore.getUiTextureAtlas(),
              playerStore.getLocalPlayerState().getRace() == Race::Greenskin ? "img_ui_1123.tga" : "img_ui_1057.tga")
    , hideInventoryOverlay(
              GameInterface::hideInventoryOverlay,
              textureStore.getUiTextureAtlas(),
              playerStore.getLocalPlayerState().getRace() == Race::Greenskin ? "img_ui_1130.tga" : "img_ui_1064.tga")
    , statsPanel(
              GameInterface::statsPanel,
              textureStore.getUiTextureAtlas(),
              playerStore.getLocalPlayerState().getRace() == Race::Greenskin ? "img_ui_1136.tga" : "img_ui_1070.tga")

    // Portrait
    , portraitRenderable(textureStore.getPortraitSpritesheet(), 1)
    , portrait(GameInterface::portrait, textureStore.getPortraitSpritesheet(), 0)

    // Text
    , nameProperties({ &fontStore.getFontSmall() })
    , nameRenderable(
              Unit::maxNameLength, nameProperties, GameInterface::selectionName.x, GameInterface::selectionName.y)
    , textRenderer(window)

    // Cursor
    , cursorRenderable(textureStore.getCursorSpritesheet(), 1)
    , cursorImage(GameInterface::cursor, textureStore.getCursorSpritesheet(), 0)
{
}

void UiRenderer::renderUi()
{
    renderMainUi();
    renderPortrait();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main UI
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UiRenderer::renderMainUi()
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
        sendMainUiDataToGpu();
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

void UiRenderer::sendMainUiDataToGpu()
{
    // First we need to know the number of images to render
    numMainUiImages = defaultNumMainUiImages;

    // Determine if the inventory should be rendered
    bool inventoryVisible = isInventoryVisible();
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

bool UiRenderer::isInventoryVisible() const
{
    auto selectedEntity = playerContext.weakSelectedEntity.lock();
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

void UiRenderer::renderPortrait()
{
    int portraitId = -1;
    bool portraitVisible = isPortraitVisible(portraitId);
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

bool UiRenderer::isPortraitVisible(int& outPortraitId) const
{
    auto selectedEntity = playerContext.weakSelectedEntity.lock();
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

void UiRenderer::renderText()
{
    textRenderables.clear();

    std::string name;
    if (isNameVisible(name))
    {
        nameRenderable.setTextSpan({ name, TextRenderable::defaultColor });
        textRenderables.push_back(&nameRenderable);
    }

    textRenderer.render(textRenderables);
}

bool UiRenderer::isNameVisible(std::string& outName) const
{
    auto selectedEntity = playerContext.weakSelectedEntity.lock();
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cursor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UiRenderer::renderCursor(int delta)
{
    // Determine the appropriate cursor (and animate!)
    CursorDef cursorDef = Cursor::getCurrentCursor(playerStore, playerContext);

    // Get the mouse position relative to the window, in pixels
    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Calculate the mouse position in the range 0-1.
    // (If the mouse is outside of the window, these values will lie outside the range).
    float normalizedMouseX = static_cast<float>(mouseX) / window.getWidth();
    float normalizedMouseY = static_cast<float>(mouseY) / window.getHeight();

    // Set the cursor image position
    cursorImage.pos.x = normalizedMouseX * RenderUtils::getMenuWidth(window.getAspectRatio());
    cursorImage.pos.y = normalizedMouseY * RenderUtils::menuHeight;

    // Offset the image based on the cursor hotspot
    cursorImage.pos.x -= cursorDef.hotspotX * RenderUtils::cursorWidthPx;
    cursorImage.pos.y -= cursorDef.hotspotY * RenderUtils::cursorHeightPx;

    // Animate
    int spriteIndex = animateCursor(cursorDef, delta);
    cursorImage.setSpriteIndex(spriteIndex);

    // Use textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cursorRenderable.getTextureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette().getId());

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
    cursorImage.addToBuffers(positions, texCoords);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, cursorRenderable.getPositionVbo());
    int positionBufferSize = positions.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, cursorRenderable.getTexCoordVbo());
    int texCoordBufferSize = texCoords.size() * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());

    // Render
    glDrawElements(cursorRenderable.getDrawMode(), cursorRenderable.getIndicesPerSprite(), GL_UNSIGNED_INT, nullptr);
}

int UiRenderer::animateCursor(const CursorDef cursorDef, int delta)
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
            cursorAnimTime += delta;

            int numFrames = cursorDef.endIndex - cursorDef.startIndex;
            int totalAnimTime = msPerCursorFrame * numFrames;
            int relativeAnimTime = cursorAnimTime % totalAnimTime;
            int relativeIndex = relativeAnimTime / msPerCursorFrame;

            return cursorDef.startIndex + relativeIndex;
        }
    }

    // Cursor has changed
    prevCursor = cursorDef;
    cursorAnimTime = 0;
    return cursorDef.startIndex;
}

}  // namespace Rival
