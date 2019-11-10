#pragma once

#include <cstdint>
#include <chrono>

#include "Core/Clock.hpp"

namespace Core {
	struct Note {
		std::uint8_t note;
		TimePoint pressed_instant, release_instant;

		inline float GetDuration() const {
			return std::chrono::duration_cast<Duration>(release_instant - pressed_instant).count();
		}
	};
}