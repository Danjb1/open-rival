#pragma once

#include "Rect.h"

namespace Rival { namespace MouseUtils {

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
