#pragma once

#include <cstdint>
#include <chrono>

#include "Core/Clock.hpp"

namespace Core {
	struct Note {
		std::uint8_t note, velocity;
		Duration on_duration, total_duration; // Duration while the note is on vs Duration before changing note (NB: it is possible that total < on )

		std::uint8_t chan{ 0 };
	};
}