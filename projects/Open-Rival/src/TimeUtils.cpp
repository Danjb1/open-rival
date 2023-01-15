#include "pch.h"

#include "TimeUtils.h"

#include <thread>

namespace Rival { namespace TimeUtils {

/** Maximum time that we are happy to spend spinning, in milliseconds. */
static constexpr Uint32 maxSpinTime = 3;

/** Maximum time that we are happy to spend yielding, in milliseconds. */
static constexpr Uint32 maxYieldTime = 10;

void PrecisionTimer::wait(Uint32 waitTimeMs)
{
    Uint32 startTime = SDL_GetTicks();
    Uint32 timeElapsed = 0;
    int timeRemaining = static_cast<int>(waitTimeMs);

    while (timeRemaining > 0)
    {
        // If there is hardly any time to wait, just spin until done.
        // This is the most reliable way to kill time but uses max CPU.
        if (timeRemaining < maxSpinTime)
        {
            // Do nothing at all - even a print statement here can
            // throw off our timing.
        }

        // If there is a little time to wait, yield to other threads.
        // Note that this will still max out the CPU.
        else if (timeRemaining < maxYieldTime)
        {
            std::this_thread::yield();
        }

        // If we have to wait any longer than that, sleep to give the CPU a break
        else
        {
            // Sleep for the shortest possible duration, just to be safe.
            // This can still cause frame drops, as sleep may take longer than requested.
            sleep(1);
        }

        Uint32 nowTime = SDL_GetTicks();
        timeElapsed = nowTime - startTime;
        timeRemaining = waitTimeMs - timeElapsed;
    }
}

}}  // namespace Rival::TimeUtils
