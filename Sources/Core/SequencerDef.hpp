#pragma once

namespace Core {
	static constexpr const unsigned int BANK_COUNT = 100;
	static constexpr const unsigned int PATTERN_COUNT = 5;
	enum class SeqMode {
		Play,
		Transpose,
		Arpegiator,
		Record,
		Stop
	};

	using Pattern = std::pair<std::vector<Note>, TimePoint /* pattern_start */>;
}