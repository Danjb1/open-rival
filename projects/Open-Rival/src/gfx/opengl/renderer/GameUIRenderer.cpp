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

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureStore.getPalette()->getId());

    // Disable transparency for the unit portraits, since they use color 0 for their background
    glUniform1i(Shaders::indexedTextureShader.transparentIndex, -1);

    const Rect& pos = GameInterface::portrait;
    portraitRenderable.setSprites({ { { pos.x, pos.y }, { pos.width, pos.height }, portraitId } });
    portraitRenderable.draw();
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

}  // namespace Rival
