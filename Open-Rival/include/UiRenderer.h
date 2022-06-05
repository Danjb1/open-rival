#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "AtlasRenderable.h"
#include "GameInterface.h"
#include "Race.h"
#include "Resources.h"
#include "Window.h"

namespace Rival {

    class UiRenderer
    {
    public:
        UiRenderer::UiRenderer(const Race& race, const Resources& res);

        void renderUi();

    private:
        static const int maxUiImages = 6;

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

        void addMainPanelToBuffers(std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords) const;

        void addStatsPanelToBuffers(std::vector<GLfloat>& positions, std::vector<GLfloat>& texCoords) const;

        bool isInventoryVisible() const;
    };

}  // namespace Rival

#endif  // UI_RENDERER_H
