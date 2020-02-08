#pragma once

#include "Core/Note.hpp"

#include <vector>

// namespace ext {
class SequenceGenerator;
// }

namespace Core {
	class Pattern;

	class Sequence {
	public:
		friend Pattern;

		Sequence(Pattern& parent);

		bool Append(SequenceGenerator& gen);
		bool Insert(SequenceGenerator& gen, std::size_t index);
		bool Mute(std::size_t index);
		bool Delete(std::size_t index);

		const std::pair<Note, bool>& GetNote(std::size_t index) const;
		std::size_t GetSize() const;

	private:
		bool CanResizeTo(std::size_t new_size) const;
		bool Resize(std::size_t new_size);

		Pattern& _parent;

		using NoteContainer = std::vector<std::pair<Note, bool /* is_(not)_muted? */ >>;

		NoteContainer _notes{};
		std::size_t _end_id{ 0 };
	};
}