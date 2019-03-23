#include "Core/Note.hpp"

#include <cctype>
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
}
