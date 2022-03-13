#include "timer.h"

Timer::Timer(const std::chrono::duration<double> timeToElaps) noexcept : m_timeToElaps{ timeToElaps }, m_startTime{ std::chrono::system_clock::now() } {}

bool Timer::hasTimeElapsed(const bool reset) noexcept {
    std::chrono::duration<double> timeElapsed = std::chrono::system_clock::now() - m_startTime;
    bool hasElapsed = timeElapsed > m_timeToElaps;
    if (reset && hasElapsed)
        m_startTime = std::chrono::system_clock::now();

    return hasElapsed;
}