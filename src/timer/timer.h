#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

#include "../utils.h"

// Timer
class Timer {
private:
    std::chrono::_V2::system_clock::time_point m_startTime;
    std::chrono::duration<double> m_timeToElaps;

public:
    // Timer.hasTimeElapsed() will return true every timeToElaps seconds
    Timer(const std::chrono::duration<double> timeToElaps) noexcept;

    // Checks if the specified time has elapsed. If it has and reset is set, the timer will be reset.
    bool hasTimeElapsed(const bool reset = true) noexcept;
};

#endif