#include "Core/Sequence.hpp"

#include "Core/Pattern.hpp"
#include "SequenceGenerator/SequenceGenerator.hpp"

#include <cassert>

namespace Core {
	Sequence::Sequence(Pattern& parent) :
		_parent { parent }
	{ }

	bool Sequence::Append(SequenceGenerator& gen) {
		const std::vector<Note>& generated = gen.Generated();

		NoteContainer new_notes{};
		for (const Note& note : generated)
			new_notes.emplace_back(note, true);

		_notes.insert(std::end(_notes), std::begin(new_notes), std::end(new_notes)); // +1 because std::insert insert before the given iterator

		_parent.SetSequenceSize(_notes.size());

		_end_id = _notes.size();
		return true;
	}

	bool Sequence::Insert(SequenceGenerator& gen, std::size_t index) {
		if (index >= _notes.size())
			return false;

		const std::vector<Note>& generated = gen.Generated();

		NoteContainer new_notes{};
		for (const Note& note : generated)
			new_notes.emplace_back(note, true);

		_notes.insert(std::begin(_notes) + index + 1, std::begin(new_notes), std::end(new_notes)); // +1 because std::insert insert before the given iterator
		
		_parent.SetSequenceSize(_notes.size());
		
		_end_id = _notes.size();
		return true;
	}

	bool Sequence::Mute(std::size_t index, bool mute) {
		if (index >= _notes.size())
			return false;

		_notes[index].second = !mute; // We mute the note
		return true;
	}

	/* Actually, a note will not be deleted unless replaced by Append function or unless all other sequences of this pattern can remove this note */
	bool Sequence::Delete(std::size_t index) {
		if (index >= _notes.size())
			return false;

		_notes[index].second = false; // We mute the note
		_end_id = index; // And change the end of this sequence
		return true;
	}

	bool Sequence::IsMuted(std::size_t index) const {
		if (index >= _notes.size())
			return false;

		return _notes[index].second;
	}

	const std::pair<Note, bool>& Sequence::GetNote(std::size_t index) const {
		assert(index < _notes.size());

		return _notes[index];
	}

	std::size_t Sequence::GetSize() const {
		return _notes.size();
	}

	bool Sequence::CanResizeTo(std::size_t new_size) const {
		return _notes.empty() || new_size >= _end_id; // >= or > ? TO CHECK // Can resize if new one is bigger
	}

	bool Sequence::Resize(std::size_t new_size) {
		if (!CanResizeTo(new_size))
			return false;

		_notes.resize(new_size, std::make_pair<Core::Note, bool>({}, false)); // Add muted notes at the end
		return true;
	}
}