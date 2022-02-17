#include "timer.h"

UpdateTimer::UpdateTimer(float64 timeToElaps) noexcept : m_timeToElaps{timeToElaps} {}

bool UpdateTimer::hasTimeElapsed(bool reset) noexcept {
    bool hasElapsed = m_timeElapsed > m_timeToElaps;
    if (reset && hasElapsed)
        m_timeElapsed = 0.0f;

    return hasElapsed;
}

void UpdateTimer::update(float64 delta) noexcept {
    m_timeElapsed += delta;
}

IndependentTimer::IndependentTimer(const std::chrono::duration<double> timeToElaps) noexcept : m_timeToElaps{timeToElaps}, m_startTime{std::chrono::system_clock::now()} {}

bool IndependentTimer::hasTimeElapsed(const bool reset) noexcept {
    std::chrono::duration<double> timeElapsed = std::chrono::system_clock::now() - m_startTime;
    bool hasElapsed = timeElapsed > m_timeToElaps;
    if (reset && hasElapsed)
        m_startTime = std::chrono::system_clock::now();

    return hasElapsed;
}