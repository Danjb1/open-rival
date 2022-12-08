#pragma once

#include "AtlasRenderable.h"
#include "GameInterface.h"
#include "MenuTextRenderer.h"
#include "Race.h"
#include "SpriteRenderable.h"
#include "Window.h"

namespace Rival {

class FontStore;
class PlayerStore;
class TextureStore;
struct PlayerContext;

class UiRenderer
{
public:
    UiRenderer::UiRenderer(
            const PlayerStore& playerStore,
            const TextureStore& textureStore,
            const FontStore& fontStore,
            const Window& window,
            const PlayerContext& playerContext);

    void renderUi();
    void renderText();
    void renderCursor();

private:
    void renderMainUi();
    bool mainUiNeedsUpdate() const;
    void sendMainUiDataToGpu();
    bool isInventoryVisible() const;

    void renderPortrait();
    bool isPortraitVisible(int& outPortraitId) const;
    bool portraitNeedsUpdate() const;
    void sendPortraitDataToGpu();

    bool isNameVisible(std::string& outName) const;

private:
    // Maximum number of images we can ever render
    static constexpr int maxMainUiImages = 6;

    // Number of images to render when nothing is selected
    static constexpr int defaultNumMainUiImages = maxMainUiImages;

    const PlayerStore& playerStore;
    const TextureStore& textureStore;
    const Window& window;
    const PlayerContext& playerContext;

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
