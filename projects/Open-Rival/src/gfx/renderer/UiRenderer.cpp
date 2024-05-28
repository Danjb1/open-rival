#include "gfx/renderer/UiRenderer.h"

#include "gfx/GlewWrapper.h"

#include "application/Resources.h"
#include "entity/Unit.h"
#include "entity/components/PortraitComponent.h"
#include "game/InventoryComponent.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"
#include "gfx/RenderUtils.h"
#include "gfx/Shaders.h"
#include "gfx/renderable/TextRenderable.h"

namespace Rival {

UiRenderer::UiRenderer(const PlayerStore& playerStore,
        const TextureStore& textureStore,
        const FontStore& fontStore,
        const Window* window,
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
    , mainPanel(GameInterface::mainPanel,
              textureStore.getUiTextureAtlas(),
              playerStore.getLocalPlayerState().getRace() == Race::Greenskin ? "img_ui_1123.tga" : "img_ui_1057.tga")
    , hideInventoryOverlay(GameInterface::hideInventoryOverlay,
              textureStore.getUiTextureAtlas(),
              playerStore.getLocalPlayerState().getRace() == Race::Greenskin ? "img_ui_1130.tga" : "img_ui_1064.tga")
    , statsPanel(GameInterface::statsPanel,
              textureStore.getUiTextureAtlas(),
              playerStore.getLocalPlayerState().getRace() == Race::Greenskin ? "img_ui_1136.tga" : "img_ui_1070.tga")

    // Portrait
    , portraitRenderable(textureStore.getPortraitSpritesheet(), 1)
    , portrait(GameInterface::portrait, textureStore.getPortraitSpritesheet(), 0)

    // Text
    , nameProperties({ &fontStore.getFontSmall() })
    , nameRenderable(
              Entity::maxNameLength, nameProperties, GameInterface::selectionName.x, GameInterface::selectionName.y)
    , textRenderer(window)

    // Cursor
    , cursorRenderer(textureStore, window)
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
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette()->getId());

    // Bind vertex array
    glBindVertexArray(mainUiRenderable.getVao());

    // Update the data on the GPU
    if (mainUiNeedsUpdate())
    {
        sendMainUiDataToGpu();
    }

    // Render
    glDrawElements(mainUiRenderable.getDrawMode(),
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
    int positionBufferSize = static_cast<int>(positions.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, mainUiRenderable.getTexCoordVbo());
    int texCoordBufferSize = static_cast<int>(texCoords.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());
}

bool UiRenderer::isInventoryVisible() const
{
    if (playerContext.weakSelectedEntities.size() != 1)
    {
        // We can only show the inventory if 1 exactly unit is selected
        return false;
    }

    auto selectedEntity = playerContext.weakSelectedEntities[0].lock();
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
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette()->getId());

    // Bind vertex array
    glBindVertexArray(portraitRenderable.getVao());

    // Update the data on the GPU
    if (portraitNeedsUpdate())
    {
        sendPortraitDataToGpu();
    }

    // Disable transparency for the unit portraits, since they use color 0 for their background
    glUniform1i(Shaders::indexedTextureShader.transparentIndex, -1);

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
    int positionBufferSize = static_cast<int>(positions.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, portraitRenderable.getTexCoordVbo());
    int texCoordBufferSize = static_cast<int>(texCoords.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());
}

bool UiRenderer::isPortraitVisible(int& outPortraitId) const
{
    if (playerContext.weakSelectedEntities.empty())
    {
        // No selection
        return false;
    }

    if (playerContext.weakSelectedEntities.size() > 1)
    {
        outPortraitId = multiSelectionPortraitId;
        return true;
    }

    auto selectedEntity = playerContext.weakSelectedEntities[0].lock();
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
    std::vector<const TextRenderable*> textRenderables;

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
    if (playerContext.weakSelectedEntities.empty())
    {
        // No selection
        return false;
    }

    if (playerContext.weakSelectedEntities.size() > 1)
    {
        // TMP
        outName = "Selection";
        return true;
    }

    auto selectedEntity = playerContext.weakSelectedEntities[0].lock();
    if (!selectedEntity)
    {
        return false;
    }

    Unit* unit = selectedEntity->as<Unit>();
    if (!unit)
    {
        return false;
    }

    outName = unit->getName();
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cursor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UiRenderer::renderCursor(int delta)
{
    CursorDef cursorDef = Cursor::getCurrentCursor(playerStore, playerContext);
    cursorRenderer.render(cursorDef, delta);
}

}  // namespace Rival
