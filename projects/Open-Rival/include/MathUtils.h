#pragma once

namespace Rival { namespace MathUtils {

int clampi(int val, int min, int max);

float clampf(float val, float min, float max);

/**
 * Rounds a number up to the nearest power of 2.
 *
 * See:
 * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
 */
int nextPowerOf2(int v);

}}  // namespace Rival::MathUtils
