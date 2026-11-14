#pragma once

#include <chrono>

namespace Rival {

/** Timing information supplied when rendering a frame. All values are in seconds. */
struct FrameTiming
{
    /** Time since the previous frame was rendered.
     * Use this to advance animations that are purely visual (e.g. the cursor). */
    float timeSinceLastRender = 0.f;

    /** Time since the game logic was last updated.
     * Use this to interpolate between logical states (e.g. smooth movement between ticks). */
    float timeSinceLastUpdate = 0.f;
};

namespace TimeUtils {

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

/** Desired FPS for the game logic. */
static constexpr int fps = 60;

/** Approximate time for each frame, in milliseconds.
 * Working with integer values is much safer than using floats, in order to keep the game simulation deterministic
 * across different systems. We don't need greater precision than this. */
static constexpr int timeStepMs = 1000 / fps;

/** Minimum time that we will consider sleeping for.
 * If the next frame is due sooner than this then we will just busy-wait, to reduce the risk of oversleeping. */
static constexpr auto minSleepTime = std::chrono::milliseconds(2);

/** Maximum number of logic updates to run before rendering a frame.
 * If the game falls further behind than this, it skips the missed updates rather than trying to catch up. */
static constexpr int maxUpdatesPerFrame = 10;

/** Upper limit for FrameTiming::timeSinceLastRender.
 * This stops visual animations from jumping ahead after a long stall (e.g. debugger breakpoint, window drag). */
static constexpr auto maxTimeSinceLastRender = std::chrono::milliseconds(250);

/** Number of ticks in the future that commands are scheduled for in a network game.
 * For now, this is quite high to avoid interruptions when latency is high.
 * Later, this should be dynamic. */
static constexpr int netCommandDelay = 20;

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

    /** Waits for some time using the most appropriate mechanism available. */
    void wait(std::chrono::duration<float> waitTime);

    /** Interrupts the timer if it is currently waiting. */
    void interrupt();

private:
    /** Sleeps for the given number of nanoseconds. */
    void sleep(long ns);

    /** Resets the interrupt flag. */
    void reset();

private:
    /** Timer handle used for sleep operations. */
    void* timerHandle;

    /** Event handle used to interrupt a sleep. */
    void* interruptHandle;

    /** Flag set when the timer is interrupted. */
    bool interrupted = false;

    /** The timer resolution in use. */
    unsigned int timerResolution = 0;
};

}  // namespace TimeUtils
}  // namespace Rival
