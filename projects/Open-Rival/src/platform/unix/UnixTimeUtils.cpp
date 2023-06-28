#ifdef __linux__

#include <unistd.h>

#include "TimeUtils.h"

namespace Rival { namespace TimeUtils {

PrecisionTimer::PrecisionTimer() {}

PrecisionTimer::~PrecisionTimer() {}

void PrecisionTimer::sleep(long ns)
{
    usleep(ns);
}

}}  // namespace Rival::TimeUtils

#endif
