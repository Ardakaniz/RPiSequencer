#pragma once

#include <chrono>

namespace Core {
	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;
	using Duration = Clock::duration;
}