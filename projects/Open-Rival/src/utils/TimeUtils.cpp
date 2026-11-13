#include "utils/TimeUtils.h"

#include <thread>

namespace Rival { namespace TimeUtils {

/** Maximum time that we are happy to spend spinning. */
static constexpr auto maxSpinTime = std::chrono::milliseconds(3);

/** Maximum time that we are happy to spend yielding. */
static constexpr auto maxYieldTime = std::chrono::milliseconds(10);

void PrecisionTimer::wait(std::chrono::duration<float> waitTime)
{
    reset();

    const auto deadline = Clock::now() + waitTime;

    while (!interrupted)
    {
        const auto remaining = std::chrono::duration<float>(deadline - Clock::now());

        if (remaining.count() <= 0.f)
        {
            break;
        }

        // If there is hardly any time to wait, just spin until done.
        // This is the most reliable way to kill time but uses max CPU.
        if (remaining < maxSpinTime)
        {
            // Do nothing at all - even a print statement here can throw off our timing
        }

        // If there is a little time to wait, yield to other threads.
        // Note that this will still max out the CPU.
        else if (remaining < maxYieldTime)
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
    }
}

}}  // namespace Rival::TimeUtils
