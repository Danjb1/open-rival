#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "AtlasRenderable.h"
#include "Resources.h"

namespace Rival {

    class UiRenderer {
    public:
        UiRenderer::UiRenderer(const Resources& res);

        void renderUi(int y) const;

    private:
        const Resources& res;
        AtlasRenderable mainPanel;

        bool needsUpdate() const;

        void sendDataToGpu(int y) const;
    };

}  // namespace Rival

#endif  // UI_RENDERER_H
