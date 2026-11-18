#include "gfx/opengl/renderer/GameUIRenderer.h"

#include "gfx/opengl/GLWrapper.h"

#include "application/Resources.h"
#include "entity/Unit.h"
#include "entity/components/PortraitComponent.h"
#include "game/InventoryComponent.h"
#include "game/PlayerContext.h"
#include "game/PlayerState.h"
#include "gfx/RenderUtils.h"
#include "gfx/opengl/Shaders.h"
#include "gfx/opengl/renderable/TextRenderable.h"

namespace Rival {

GameUIRenderer::GameUIRenderer(const PlayerStore& playerStore,
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
    , nameProperties({ fontStore.getFontSmall() })
    , textRenderer(window)
{
}

void GameUIRenderer::renderUi()
{
    renderMainUi();
    renderPortrait();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main UI
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameUIRenderer::renderMainUi()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette()->getId());

    std::vector<AtlasImage> images;
    images.reserve(maxMainUiImages);
    images.push_back(toAtlasImage(minimapLeftBorder));
    images.push_back(toAtlasImage(minimapTopBorder));
    images.push_back(toAtlasImage(minimapBottomBorder));
    images.push_back(toAtlasImage(mainPanel));
    if (!isInventoryVisible())
    {
        // It's counter-intuitive, but to show the inventory we *don't* draw the overlay
        images.push_back(toAtlasImage(hideInventoryOverlay));
    }
    images.push_back(toAtlasImage(statsPanel));

    mainUiRenderable.setImages(images);
    mainUiRenderable.draw();
}

bool GameUIRenderer::isInventoryVisible() const
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

void GameUIRenderer::renderPortrait()
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

bool GameUIRenderer::portraitNeedsUpdate() const
{
    // Later, perhaps we can optimise this
    return true;
}

void GameUIRenderer::sendPortraitDataToGpu()
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
    addToBuffers(portrait, positions, texCoords);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, portraitRenderable.getPositionVbo());
    int positionBufferSize = static_cast<int>(positions.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionBufferSize, positions.data());

    // Upload tex co-ord data
    glBindBuffer(GL_ARRAY_BUFFER, portraitRenderable.getTexCoordVbo());
    int texCoordBufferSize = static_cast<int>(texCoords.size() * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordBufferSize, texCoords.data());
}

bool GameUIRenderer::isPortraitVisible(int& outPortraitId) const
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

void GameUIRenderer::renderText()
{
    std::vector<const TextRenderable*> textRenderables;

    std::string name;
    if (isNameVisible(name))
    {
        nameRenderable.setText({ { name, TextRenderable::defaultColor } },
                nameProperties,
                GameInterface::selectionName.x,
                GameInterface::selectionName.y);
        textRenderables.push_back(&nameRenderable);
    }

    textRenderer.render(textRenderables);
}

bool GameUIRenderer::isNameVisible(std::string& outName) const
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
// Helpers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AtlasImage GameUIRenderer::toAtlasImage(const GameInterface::UiImage& image)
{
    const Rect pos = image.getPos();
    return { { pos.x, pos.y }, { pos.width, pos.height }, image.getImageKey() };
}

void GameUIRenderer::addToBuffers(
        const GameInterface::UiImage& image, std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords)
{
    // Define vertex positions
    Rect pos = image.getPos();
    float x1 = pos.x;
    float y1 = pos.y;
    float x2 = x1 + pos.width;
    float y2 = y1 + pos.height;
    float z = 0;

    /* clang-format off */
    std::vector<GLfloat> thisVertexData = {
        x1, y1, z,
        x2, y1, z,
        x2, y2, z,
        x1, y2, z
    };
    /* clang-format on */

    // Determine texture co-ordinates
    std::vector<GLfloat> thisTexCoords;
    if (image.getTexAtlas())
    {
        thisTexCoords = image.getTexAtlas()->getTexCoords(image.getImageKey());
    }
    else if (image.getSpritesheet())
    {
        thisTexCoords = image.getSpritesheet()->getTexCoords(image.getSpriteIndex());
    }
    else
    {
        const float tx1 = 0.f;
        const float tx2 = 1.f;
        const float ty1 = 0.f;
        const float ty2 = 1.f;

        /* clang-format off */
        thisTexCoords = {
            tx1, ty1,
            tx2, ty1,
            tx2, ty2,
            tx1, ty2
        };
        /* clang-format on */
    }

    // Copy this data into the main buffers
    positions.insert(positions.cend(), thisVertexData.cbegin(), thisVertexData.cend());
    texCoords.insert(texCoords.cend(), thisTexCoords.cbegin(), thisTexCoords.cend());
}

}  // namespace Rival
