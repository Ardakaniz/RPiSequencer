#pragma once

#include <chrono>

namespace Core {
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	using Duration = std::chrono::duration<float>; // In seconds, float to have more precision
}