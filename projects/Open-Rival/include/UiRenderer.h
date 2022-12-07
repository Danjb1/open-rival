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
    void renderText(const Selection& selection);
    void renderCursor();

private:
    void renderMainUi(const Selection& selection);
    bool mainUiNeedsUpdate() const;
    void sendMainUiDataToGpu(const Selection& selection);
    bool isInventoryVisible(const Selection& selection) const;

    void renderPortrait(const Selection& selection);
    bool isPortraitVisible(const Selection& selection, int& outPortraitId) const;
    bool portraitNeedsUpdate() const;
    void sendPortraitDataToGpu();

    bool isNameVisible(const Selection& selection, std::string& outName) const;

private:
    // Maximum number of images we can ever render
    static constexpr int maxMainUiImages = 6;

    // Number of images to render when nothing is selected
    static constexpr int defaultNumMainUiImages = maxMainUiImages;

    const TextureStore& textureStore;
    const Window& window;

    // Main UI
    int numMainUiImages = 0;
    GameInterface::UiImage minimapLeftBorder;
    GameInterface::UiImage minimapTopBorder;
    GameInterface::UiImage minimapBottomBorder;
    GameInterface::UiImage mainPanel;
    GameInterface::UiImage hideInventoryOverlay;
    GameInterface::UiImage statsPanel;
    AtlasRenderable mainUiRenderable;

    // Portrait
    GameInterface::UiImage portrait;
    SpriteRenderable portraitRenderable;

    // Text
    std::vector<TextRenderable*> textRenderables;
    TextProperties nameProperties;
    TextRenderable nameRenderable;
    MenuTextRenderer textRenderer;

    // Cursor
    GameInterface::UiImage cursorImage;
    SpriteRenderable cursorRenderable;
};

}  // namespace Rival
