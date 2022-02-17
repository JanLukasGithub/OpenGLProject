#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

#include "../utils.h"

// Abstract Timer interface
class Timer {
public:
    // Checks if the specified time has elapsed. If it has and reset is set, the timer will be reset.
    virtual bool hasTimeElapsed(bool reset) noexcept = 0;
};

// Timer that needs to be updated regularly
class UpdateTimer : Timer {
public:
    // Timer.hasTimeElapsed() will return true every timeToElaps seconds
    UpdateTimer(const float64 timeToElaps) noexcept;

    // Checks if the specified time has elapsed. If it has and reset is set, the timer will be reset.
    virtual bool hasTimeElapsed(const bool reset = true) noexcept override;

    // Updates the timer. Delta is measured in seconds. 
    void update(const float64 delta) noexcept;
private:
    float64 m_timeElapsed = 0.0f;
    float64 m_timeToElaps = 0.0f;
};

// Timer that doesn't need to be updated regularly
class IndependentTimer : Timer {
public:
    // Timer.hasTimeElapsed() will return true every timeToElaps seconds
    IndependentTimer(const std::chrono::duration<double> timeToElaps) noexcept;

    // Checks if the specified time has elapsed. If it has and reset is set, the timer will be reset.
    virtual bool hasTimeElapsed(const bool reset = true) noexcept override;
private:
    std::chrono::_V2::system_clock::time_point m_startTime;
    std::chrono::duration<double> m_timeToElaps;
};

#endif