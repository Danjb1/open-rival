#include "pch.h"
#include "MouseUtils.h"

namespace Rival { namespace MouseUtils {

    int getMouseInViewportX(int mouseX, const Rect viewport) {
        return mouseX - static_cast<int>(viewport.x);
    }

    int getMouseInViewportY(int mouseY, const Rect viewport) {
        return mouseY - static_cast<int>(viewport.y);
    }

    float getNormalizedMouseInViewportX(int mouseX, const Rect viewport) {
        int mouseInViewportX = getMouseInViewportX(mouseX, viewport);
        return static_cast<float>(mouseInViewportX) / viewport.width;
    }

    float getNormalizedMouseInViewportY(int mouseY, const Rect viewport) {
        int mouseInViewportY = getMouseInViewportY(mouseY, viewport);
        return static_cast<float>(mouseInViewportY) / viewport.height;
    }

}}  // namespace Rival::MouseUtils
