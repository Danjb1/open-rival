#ifdef _WIN32

// These comments...
#include "utils/TimeUtils.h"
// ... prevent the auto-formatter from moving the include

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <timeapi.h>

#include <chrono>
#include <thread>

namespace Rival { namespace TimeUtils {

bool nanosleep(HANDLE timer, LONGLONG ns)
{
    if (!timer)
    {
        return false;
    }

    // Set timer properties
    LARGE_INTEGER li {};
    li.QuadPart = -ns;
    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE))
    {
        return false;
    }

    // Start and wait for timer
    WaitForSingleObject(timer, INFINITE);

    return true;
}

PrecisionTimer::PrecisionTimer()
{
    /*
     * While in scope, this timer forces the lowest possible timer resolution.
     * See: https//docs.microsoft.com/en-gb/windows/win32/multimedia/obtaining-and-setting-timer-resolution
     */

    // Set timer resolution
    TIMECAPS tc;
    if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR)
    {
        timerResolution = tc.wPeriodMin;
        timeBeginPeriod(timerResolution);
    }

    // Create a timer
    timer = CreateWaitableTimer(NULL, TRUE, NULL);
}

PrecisionTimer::~PrecisionTimer()
{
    // Reset timer resolution
    if (timerResolution > 0)
    {
        timeEndPeriod(timerResolution);
    }

    // Clean up resources
    if (timer)
    {
        CloseHandle(timer);
    }
}

void PrecisionTimer::sleep(long ns)
{
    if (nanosleep(timer, ns))
    {
        return;
    }

    // Default cross-platform sleep, if all else fails
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

}}  // namespace Rival::TimeUtils

#endif
