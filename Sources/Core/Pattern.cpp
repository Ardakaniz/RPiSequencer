#include "Core/Pattern.hpp"

namespace Core
{
	void Pattern::Clear()
	{ 
		_notes.clear();
		_step = 0;
	}

	void Pattern::Step()
	{ 
		++_step;

		if (_step >= _notes.size())
			_step = 0;
	}

	const Note& Pattern::GetNote() const
	{ return _notes[_step]; }
}
