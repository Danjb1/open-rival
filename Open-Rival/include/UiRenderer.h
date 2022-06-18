#pragma once

#include "AtlasRenderable.h"
#include "GameInterface.h"
#include "Race.h"

namespace Rival {

class TextureStore;

class UiRenderer
{
public:
    UiRenderer::UiRenderer(const Race& race, const TextureStore& textureStore);

    void renderUi();

private:
    bool needsUpdate() const;

    void sendDataToGpu();

    bool isInventoryVisible() const;

private:
    static constexpr int maxUiImages = 6;

    const TextureStore& textureStore;

    int numUiImages;
    AtlasRenderable mainUiRenderable;

    GameInterface::UiImage minimapLeftBorder;
    GameInterface::UiImage minimapTopBorder;
    GameInterface::UiImage minimapBottomBorder;
    GameInterface::UiImage mainPanel;
    GameInterface::UiImage inventoryOverlay;
    GameInterface::UiImage statsPanel;
};

}  // namespace Rival
