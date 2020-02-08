#pragma once

#include <vector>

#include "Core/Note.hpp"

namespace Core {
	static constexpr const unsigned int BANK_SIZE = 100;
	static constexpr const unsigned int PATTERN_SIZE = 5;
	enum SeqMode {
		SeqMode_Play,
		SeqMode_Transpose,
		SeqMode_Stop,

		SeqMode_Count = SeqMode_Stop,

		/* ADDITIONNAL MODE FLAGS */ // First two bits reserved to generic mode
		SeqModeFlag_Stepper = 0b100, 

		SeqModeFlag_Max = SeqModeFlag_Stepper,
		SeqModeFlag_Mask = ~0b11,
	};
}