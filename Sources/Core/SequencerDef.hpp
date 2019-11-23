#pragma once

#include <vector>

#include "Core/Note.hpp"

namespace Core {
	static constexpr const unsigned int BANK_COUNT = 100;
	static constexpr const unsigned int PATTERN_COUNT = 5;
	enum SeqMode {
		SeqMode_Play,
		SeqMode_Transpose,
		SeqMode_Arpegiator,
		SeqMode_Record,
		SeqMode_Stop,

		SeqMode_Count = SeqMode_Stop,

		/* ADDITIONNAL MODE FLAGS */
		SeqModeFlag_Stepper = 0x10,

		SeqModeFlag_Max = SeqModeFlag_Stepper,
		SeqModeFlag_Mask = 0xf0,
	};

	using Pattern = std::pair<std::vector<Note>, TimePoint /* pattern_start */>;
}