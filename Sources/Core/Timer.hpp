#pragma once

#ifndef CORE_TIMER_HPP
#define CORE_TIMER_HPP

#include <chrono>

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Seconds = std::chrono::duration<float>; // To have more precision

#endif // CORE_TIMER_HPP