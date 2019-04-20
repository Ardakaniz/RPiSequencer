#include "Core/Note.hpp"

#include <stdexcept>

namespace Core
{
	Note::Note() :
		_note { 0 },
		_duration{ 0.f }
	{ }

	Note::Note(unsigned int note, float duration) :
		_note{ note },
		_duration{ duration }
	{ }

	Note::Note(const Note& note, int offset) :
		_note{ note._note + offset },
		_duration{ note._duration }
	{ }

	int Note::Offset(const Note& other) const
	{ return static_cast<int>(other._note) - static_cast<int>(_note); } // Cast because we can have a negative offset

	bool Note::IsBlankNote() const
	{ return _duration == 0.f; }

	float Note::GetDuration() const
	{ return _duration; }
}
