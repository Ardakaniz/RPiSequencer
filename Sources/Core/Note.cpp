#include "Core/Note.hpp"

#include <cctype>
#include <stdexcept>

namespace Core
{
	Note::Note() :
		_is_blank_note{ true },
		_note { 'C' },
		_octave{ 0 }
	{ }

	Note::Note(char note, unsigned int octave) :
		_is_blank_note{ false },
		_note{ note },
		_octave{ octave }
	{ }

	Note::Note(const std::string& note)
	{
		_is_blank_note = (note == "*");
		if (!_is_blank_note)
		{
			if (note.length() != 2)
				ThrowInvalidNoteString();

			if (!std::isalpha(note[0]))
				ThrowInvalidNoteString();
			_note = note[0];

			if (!std::isdigit(note[1]))
				ThrowInvalidNoteString();
			_octave = static_cast<unsigned int>(note[1] - '0');
		}		
	}

	void Note::ThrowInvalidNoteString()
	{ throw std::runtime_error{ "Invalid note string" }; }
}