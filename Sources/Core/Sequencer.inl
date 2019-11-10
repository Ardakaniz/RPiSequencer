namespace Core {
	const Pattern& Sequencer::GetPattern() const {
		return GetPattern(_pattern_index, _bank_index);
	}

	Pattern& Sequencer::GetPattern() {
		return GetPattern(_pattern_index, _bank_index);
	}

	const Pattern& Sequencer::GetPattern(unsigned int pattern_index, unsigned int bank_index) const {
		return _notes[bank_index * BANK_COUNT + pattern_index];
	}

	Pattern& Sequencer::GetPattern(unsigned int pattern_index, unsigned int bank_index) {
		return _notes[bank_index * BANK_COUNT + pattern_index];
	}
}