#include "Core/Pattern.hpp"

namespace Core {
	void Pattern::ChangeSequence(std::size_t id) {
		_current_sequence = id;
	}

	bool Pattern::SetSequenceSize(std::size_t new_size) {
		// If any sequence can't be resized, we dont resize any one
		if (std::any_of(std::cbegin(_sequences), std::cend(_sequences), [new_size](const Sequence& seq) { return !seq.CanResizeTo(new_size); }))
			return false;

		for (auto& sequence : _sequences)
			sequence.Resize(new_size);

		return true;
	}

	Sequence& Pattern::GetSequence() {
		return _sequences[_current_sequence];
	}

	const Sequence& Pattern::GetSequence() const {
		return _sequences[_current_sequence];
	}
}