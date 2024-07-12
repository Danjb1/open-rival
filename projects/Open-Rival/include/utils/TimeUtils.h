#pragma once

#include "utils/SDLWrapper.h"

namespace Rival { namespace TimeUtils {

/** Desired FPS for the game logic. */
static constexpr int fps = 60;

/** Approximate time for each frame, in milliseconds. */
static constexpr int timeStepMs = 1000 / fps;

/**
 * Minimum time that we will consider sleeping for.
 *
 * If the next frame is due sooner than this then we will just
 * busy-wait, to reduce the risk of oversleeping.
 */
static constexpr int minSleepTime = 2;

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

    /** Waits for some time (seconds) using the most appropriate mechanism available. */
    void wait(Uint32 waitTimeMs);

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

}}  // namespace Rival::TimeUtils
