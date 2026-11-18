#pragma once

#include "game/GameInterface.h"
#include "game/Race.h"
#include "gfx/opengl/renderable/AtlasRenderable.h"
#include "gfx/opengl/renderable/SpriteRenderable.h"
#include "gfx/opengl/renderer/MenuTextRenderer.h"
#include "window/Window.h"

namespace Rival {

class FontStore;
class PlayerStore;
class TextureStore;
struct PlayerContext;

/**
 * Class responsible for rendering the in-game UI.
 */
class GameUIRenderer
{
public:
    GameUIRenderer(const PlayerStore& playerStore,
            const TextureStore& textureStore,
            const FontStore& fontStore,
            const Window* window,
            const PlayerContext& playerContext);

    void renderUi();
    void renderText();

private:
    static AtlasImage toAtlasImage(const GameInterface::UiImage& image);

    void renderMainUi();
    bool isInventoryVisible() const;

    void renderPortrait();
    bool isPortraitVisible(int& outPortraitId) const;

    bool isNameVisible(std::string& outName) const;

private:
    // Maximum number of images we can ever render
    static constexpr int maxMainUiImages = 6;

    // TMP: Portrait to show when multiple units are selected
    static constexpr int multiSelectionPortraitId = 108;

    const PlayerStore& playerStore;
    const TextureStore& textureStore;
    const Window* window;
    const PlayerContext& playerContext;

    // Main UI
    GameInterface::UiImage minimapLeftBorder;
    GameInterface::UiImage minimapTopBorder;
    GameInterface::UiImage minimapBottomBorder;
    GameInterface::UiImage mainPanel;
    GameInterface::UiImage hideInventoryOverlay;
    GameInterface::UiImage statsPanel;
    AtlasRenderable mainUiRenderable;

    // Portrait
    SpriteRenderable portraitRenderable;

    // Text
    TextProperties nameProperties;
    TextRenderable nameRenderable;
    MenuTextRenderer textRenderer;
};

}  // namespace Rival
