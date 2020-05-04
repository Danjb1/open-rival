#include "TimerUtils.h"

namespace Rival {

    namespace TimerUtils {

        MillisecondsType getCurrentTime() {
            const auto now(std::chrono::system_clock::now());
            const auto nowMS(std::chrono::duration_cast<MillisecondsType>(now.time_since_epoch()));
            return nowMS - gameStartTime;
        }

        MillisecondsType getTimeDifference(TimeType t1, TimeType t2) {
            const auto tDiff = t2 - t1;
            auto retVal(std::chrono::duration_cast<MillisecondsType>(tDiff));
            return retVal;
        }

        MillisecondsType getTimeSince(MillisecondsType t) {
            return gameStartTime - t;
        }

        MillisecondsRep toMS(uint32_t t) {
            return MillisecondsType(t);
        }

    }

}
