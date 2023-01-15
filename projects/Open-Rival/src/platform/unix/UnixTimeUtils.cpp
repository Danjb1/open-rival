#include "pch.h"

#ifdef __linux__

#include <unistd.h>

#include "TimeUtils.h"

namespace Rival { namespace TimeUtils {

void sleep(int ms)
{
    usleep((unsigned long) (milliseconds * 1000.0));
}

}}  // namespace Rival::TimeUtils

#endif
