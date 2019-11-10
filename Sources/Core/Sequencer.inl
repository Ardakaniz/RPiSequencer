namespace Core {
	const std::pair<std::vector<Note>, TimePoint>& Sequencer::GetPattern() const {
		return GetPattern(_pattern_index, _bank_index);
	}

	std::pair<std::vector<Note>, TimePoint>& Sequencer::GetPattern() {
		return GetPattern(_pattern_index, _bank_index);
	}

	const std::pair<std::vector<Note>, TimePoint>& Sequencer::GetPattern(unsigned int pattern_index, unsigned int bank_index) const {
		return _notes[static_cast<std::size_t>(bank_index * BANK_COUNT + pattern_index)];
	}

	std::pair<std::vector<Note>, TimePoint>& Sequencer::GetPattern(unsigned int pattern_index, unsigned int bank_index) {
		return _notes[static_cast<std::size_t>(bank_index * BANK_COUNT + pattern_index)];
	}
}