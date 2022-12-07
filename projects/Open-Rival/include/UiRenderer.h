#pragma once

#include "AtlasRenderable.h"
#include "GameInterface.h"
#include "MenuTextRenderer.h"
#include "Race.h"
#include "SpriteRenderable.h"
#include "Window.h"

namespace Rival {

class FontStore;
class TextureStore;
struct Selection;

class UiRenderer
{
public:
    UiRenderer::UiRenderer(
            const Race& race, const TextureStore& textureStore, const FontStore& fontStore, const Window& window);

    void renderUi(const Selection& selection);

private:
    void renderMainUi(const Selection& selection);
    bool mainUiNeedsUpdate() const;
    void sendMainUiDataToGpu(const Selection& selection);
    bool isInventoryVisible(const Selection& selection) const;

    void renderPortrait(const Selection& selection);
    bool isPortraitVisible(const Selection& selection, int& outPortraitId) const;
    bool portraitNeedsUpdate() const;
    void sendPortraitDataToGpu();

    void renderText(const Selection& selection);
    bool isNameVisible(const Selection& selection, std::string& outName) const;

private:
    // Maximum number of images we can ever render
    static constexpr int maxMainUiImages = 6;

    // Number of images to render when nothing is selected
    static constexpr int defaultNumMainUiImages = maxMainUiImages;

    const TextureStore& textureStore;

    int numMainUiImages = 0;
    AtlasRenderable mainUiRenderable;

    SpriteRenderable portraitRenderable;

    std::vector<TextRenderable*> textRenderables;
    TextProperties nameProperties;
    TextRenderable nameRenderable;
    MenuTextRenderer textRenderer;

    GameInterface::UiImage minimapLeftBorder;
    GameInterface::UiImage minimapTopBorder;
    GameInterface::UiImage minimapBottomBorder;
    GameInterface::UiImage mainPanel;
    GameInterface::UiImage hideInventoryOverlay;
    GameInterface::UiImage portrait;
    GameInterface::UiImage statsPanel;
};

}  // namespace Rival
