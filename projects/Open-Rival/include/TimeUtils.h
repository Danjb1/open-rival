#pragma once

#include "SDLWrapper.h"

namespace Rival { namespace TimeUtils {

static constexpr int fps = 60;

static constexpr int timeStepMs = 1000 / fps;

/**
 * Allows for high-precision sleep timers.
 *
 * We have to rely on OS-specific methods for high-precision sleeps.
 * See: https://stackoverflow.com/a/41862592/1624459
 */
class PrecisionTimer
{
public:
    PrecisionTimer();
    ~PrecisionTimer();

    /** Waits for some time (seconds) using the most appropriate mechanism available. */
    void wait(Uint32 waitTimeMs);

private:
    /** Sleeps for the given number of nanoseconds. */
    void sleep(long ns);

private:
    /** Timer handle used for sleep operations. */
    void* timer;

    /** The timer resolution in use. */
    unsigned int timerResolution = 0;
};

}}  // namespace Rival::TimeUtils
