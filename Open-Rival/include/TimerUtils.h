#ifndef TIMER_UTILS_H
#define TIMER_UTILS_H

#include <chrono>

namespace Rival {
    namespace TimerUtils {

        static const int timeStepMs = 1000 / 60;

        using MillisecondsType = std::chrono::milliseconds;
        using MillisecondsRep = decltype(MillisecondsType());
        using TimeType = decltype(std::chrono::system_clock::now());

        static const auto gameStartTime(
            std::chrono::duration_cast<MillisecondsType>(
                std::chrono::system_clock::now().time_since_epoch()));
        MillisecondsType getCurrentTime();

        // Return the time passed from t1 to t2
        MillisecondsType getTimeDifference(TimeType t1, TimeType t2);

        // Return the time passed since t
        MillisecondsType getTimeSince(MillisecondsType t);

        MillisecondsRep toMS(uint32_t t);

    }
}

#endif // TIMER_UTILS_H
