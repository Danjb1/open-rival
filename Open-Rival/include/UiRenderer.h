#pragma once

#include "AtlasRenderable.h"
#include "GameInterface.h"
#include "Race.h"

namespace Rival {

class Resources;

class UiRenderer
{
public:
    UiRenderer::UiRenderer(const Race& race, const Resources& res);

    void renderUi();

private:
    static constexpr int maxUiImages = 6;

    const Resources& res;

    int numUiImages;
    AtlasRenderable mainUiRenderable;

    GameInterface::UiImage minimapLeftBorder;
    GameInterface::UiImage minimapTopBorder;
    GameInterface::UiImage minimapBottomBorder;
    GameInterface::UiImage mainPanel;
    GameInterface::UiImage inventoryOverlay;
    GameInterface::UiImage statsPanel;

    bool needsUpdate() const;

    void sendDataToGpu();

    bool isInventoryVisible() const;
};

}  // namespace Rival
