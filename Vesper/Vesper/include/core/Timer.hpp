#pragma once

#include <chrono>

template <typename TimeUnit>
class Timer
{
public:
    Timer();
    ~Timer();

    TimeUnit getElapsedTime() const;
    TimeUnit restart();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_timePoint;
};

template <typename TimeUnit>
inline Timer<TimeUnit>::Timer()
    : m_timePoint(std::chrono::high_resolution_clock::now())
{
}

template<typename TimeUnit>
inline Timer<TimeUnit>::~Timer()
{
}

template<typename TimeUnit>
inline TimeUnit Timer<TimeUnit>::getElapsedTime() const
{
    return std::chrono::duration_cast<TimeUnit>(std::chrono::high_resolution_clock::now() - m_timePoint);
}

template<typename TimeUnit>
inline TimeUnit Timer<TimeUnit>::restart()
{
    const TimeUnit elapsed(getElapsedTime());
    m_timePoint = std::chrono::high_resolution_clock::now();
    return elapsed;
}

using NanoTimer = Timer<std::chrono::nanoseconds>;
using MicroTimer = Timer<std::chrono::microseconds>;
using MilliTimer = Timer<std::chrono::milliseconds>;