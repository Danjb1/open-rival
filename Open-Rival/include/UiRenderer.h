#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "AtlasRenderable.h"
#include "Resources.h"
#include "Window.h"

namespace Rival {

    class UiRenderer {
    public:
        UiRenderer::UiRenderer(const Resources& res, const Window& window);

        void renderUi();

    private:
        static const int maxUiImages = 2;

        const Resources& res;
        const Window& window;

        int numUiImages;
        AtlasRenderable mainUiRenderable;

        bool needsUpdate() const;

        void sendDataToGpu();

        void addMainPanelToBuffers(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords) const;

        void addStatsPanelToBuffers(
                std::vector<GLfloat>& positions,
                std::vector<GLfloat>& texCoords) const;
    };

}  // namespace Rival

#endif  // UI_RENDERER_H
