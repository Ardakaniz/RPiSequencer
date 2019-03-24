#include "Core/Note.hpp"

#include <stdexcept>

namespace Core
{
	Note::Note() :
		_is_blank_note{ true },
		_note { 0 }
	{ }

	Note::Note(unsigned int note) :
		_is_blank_note{ false },
		_note{ note }
	{ }

	Note::Note(const Note& note, int offset) :
		_is_blank_note{ note._is_blank_note },
		_note{ note._note + offset }
	{ }

	int Note::Offset(const Note& other) const
	{ 
		if (_is_blank_note || other._is_blank_note)
			throw std::runtime_error{ "Invalid offset with blank note" };

		return static_cast<int>(other._note) - static_cast<int>(_note);
	}
}
