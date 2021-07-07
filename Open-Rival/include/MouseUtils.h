#ifndef MOUSE_UTILS_H
#define MOUSE_UTILS_H

#include "Rect.h"

namespace Rival {
namespace MouseUtils {

    /**
     * Calculates the mouse position relative to the viewport, in pixels.
     */
    int getMouseInViewportX(int mouseX, const Rect viewport);

    /**
     * Calculates the mouse position relative to the viewport, in pixels.
     */
    int getMouseInViewportY(int mouseY, const Rect viewport);

    /**
     * Calculates the mouse position relative to the viewport, in the range 0-1.
     */
    float getNormalizedMouseInViewportX(int mouseX, const Rect viewport);

    /**
     * Calculates the mouse position relative to the viewport, in the range 0-1.
     */
    float getNormalizedMouseInViewportY(int mouseY, const Rect viewport);

}}  // namespace Rival::MouseUtils

#endif  // MOUSE_UTILS_H
