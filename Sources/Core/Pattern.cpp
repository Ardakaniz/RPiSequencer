#include "Core/Pattern.hpp"

#include <algorithm>

namespace Core
{
	Pattern::Pattern(Platform::MidiInterface& midi_interface) :
		_midi_interface{ midi_interface },
		_last_step{ Clock::now() }
	{ }

	void Pattern::Clear()
	{ 
		_notes.clear();
		_step = 0;
	}

	void Pattern::Run()
	{
		TimePoint now = Clock::now();

		for (auto it = std::rbegin(_notes_on); it != std::rend(_notes_on); ++it)
		{
			const float since_on = std::chrono::duration_cast<Seconds>(now - it->second).count();

			if (since_on >= it->first.GetDuration())
			{
				Platform::MidiNote::Off(_midi_interface, it->first);
				_notes_on.pop_back();
			}
		}
		
		if (_intervals.empty())
			return;

		now = Clock::now();
		const float since_last_step = std::chrono::duration_cast<Seconds>(now - _last_step).count();
		if (since_last_step >= _intervals[_step])
		{
			_last_step = now;
			
			Note note{ _notes[_step], _offset };
			Platform::MidiNote::On(_midi_interface, note);
			_notes_on.emplace_back(note, Clock::now());
			
			Step();
		}
	}
	
	void Pattern::Trigger()
	{
		if (!_notes.empty())
		{
			Note note{ _notes[_step], _offset };
			Platform::MidiNote::Trigger(_midi_interface, note);
		}
		
		Step();
	}
	
	void Pattern::Stop()
	{
		for (const auto& note : _notes_on)
			Platform::MidiNote::Off(_midi_interface, note.first);
		_notes_on.clear();
		
		Reset();
	}

	void Pattern::Reset()
	{ _step = 0; }

	void Pattern::SetOffset(int offset)
	{ _offset = offset; }
	
	const Note& Pattern::GetNote() const
	{ return _notes[_step]; }
	
	bool Pattern::IsEmpty() const
	{ return _notes.empty(); }
	
	void Pattern::Step()
	{
		++_step;

		if (_step >= _notes.size())
			_step = 0;
	}
}
