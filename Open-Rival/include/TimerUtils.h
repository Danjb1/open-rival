#ifndef TIMER_UTILS_H
#define TIMER_UTILS_H

#include <chrono>

namespace Rival {
namespace TimerUtils {

    static const int fps = 60;

    static const int timeStepMs = 1000 / fps;

}}  // namespace Rival::TimerUtils

#endif  // TIMER_UTILS_H
