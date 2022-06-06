#include "pch.h"

#include "MathUtils.h"

namespace Rival { namespace MathUtils {

    int clampi(int val, int min, int max)
    {
        if (val < min)
        {
            return min;
        }
        else if (val > max)
        {
            return max;
        }
        return val;
    }

    float clampf(float val, float min, float max)
    {
        if (val < min)
        {
            return min;
        }
        else if (val > max)
        {
            return max;
        }
        return val;
    }

}}  // namespace Rival::MathUtils
