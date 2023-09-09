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

#include "utils/LogUtils.h"

namespace Rival { namespace TimeUtils {

bool nanosleep(HANDLE timerHandle, HANDLE interruptHandle, LONGLONG ns)
{
    if (!timerHandle || !interruptHandle)
    {
        return false;
    }

    // Set up timer
    LARGE_INTEGER li {};
    li.QuadPart = -ns;

    // Activate the timer
    if (!SetWaitableTimer(timerHandle, &li, 0, NULL, NULL, FALSE))
    {
        return false;
    }

    // Wait for timer to complete, or be interrupted
    std::vector<HANDLE> handles;
    handles.push_back(timerHandle);
    handles.push_back(interruptHandle);
    WaitForMultipleObjects(
            static_cast<DWORD>(handles.size()),
            handles.data(),
            /* WaitAll = */ FALSE,
            /* Timeout = */ INFINITE);

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

    // Create our timer
    timerHandle = CreateWaitableTimer(NULL, TRUE, NULL);

    // Create our interrupt event
    interruptHandle = CreateEventA(NULL, /* ManualReset = */ TRUE, /* Signalled = */ FALSE, NULL);
}

PrecisionTimer::~PrecisionTimer()
{
    // Reset timer resolution
    if (timerResolution > 0)
    {
        timeEndPeriod(timerResolution);
    }

    // Clean up resources
    if (timerHandle)
    {
        CloseHandle(timerHandle);
    }
}

void PrecisionTimer::sleep(long ns)
{
    if (nanosleep(timerHandle, interruptHandle, ns))
    {
        return;
    }

    // Default cross-platform sleep, if all else fails
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void PrecisionTimer::interrupt()
{
    if (!SetEvent(interruptHandle))
    {
        LOG_WARN("Failed to interrupt timer: {}\n", GetLastError());
    }

    interrupted = true;
}

void PrecisionTimer::reset()
{
    if (!interrupted)
    {
        return;
    }

    if (!ResetEvent(interruptHandle))
    {
        LOG_WARN("Failed to reset timer: {}\n", GetLastError());
    }

    interrupted = false;
}

}}  // namespace Rival::TimeUtils

#endif
