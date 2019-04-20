#include "Core/Sequencer.hpp"

#include <stdexcept>
#include <iostream>

namespace Core
{
	Sequencer::Sequencer() :
		_last_tap{ Clock::now() },
		_last_note{ Clock::now() }
	{
		_patterns.emplace_back(_midi_interface); // Create one first blank pattern so that _pattern_index is not out of bound
		
		_no_valid_interface = _midi_interface.GetInterfacesName().empty();
	}
	
	std::vector<std::string> Sequencer::GetInterfacesName()
	{ 
		_no_valid_interface = !_midi_interface.UpdateInterfaces();
		return _midi_interface.GetInterfacesName();
	}
	
	void Sequencer::SelectInterface(unsigned int id)
	{
		if (!_no_valid_interface)
			_midi_interface.SelectInterface(id);
	}
	
	void Sequencer::CloseInterface()
	{ _midi_interface.CloseInterface(); }
		

	void Sequencer::OnTap()
	{
		switch (_state)
		{
			case State::Transpose:
			case State::Play:
			case State::Stop: // We tap the tempo
			{
				const TimePoint now = Clock::now();
				const float since_last_tap = std::chrono::duration_cast<Seconds>(now - _last_tap).count();

				_rate = since_last_tap;
				_last_tap = now;
				break;
			}

			case State::Record: // We add a blank note that last 0 second
				const TimePoint now = Clock::now();
				const float interval = std::chrono::duration_cast<Seconds>(now - _last_note).count();
				
				_last_note = now;
				
				_patterns[GetCurrentPatternIndex()].AddNote(interval, 0, 0.f);
				break;
		}
	}

	void Sequencer::Run()
	{
		if (_no_valid_interface)
		{
			_no_valid_interface = !_midi_interface.UpdateInterfaces();
			return;
		}
		
		_midi_interface.Poll();

		switch (_state)
		{
			case State::Transpose: // We change the transpose offset when a note is triggered
			{
				Note note{};
				if (Platform::MidiNote::Read(_midi_interface, note, false))
				{
					Pattern& current_pattern{ _patterns[GetCurrentPatternIndex()] };
					
					current_pattern.Reset();
					do
					{
						current_pattern.SetOffset(current_pattern.GetNote().Offset(note));
					}
					while (current_pattern.GetNote().IsBlankNote()); // If it was a blank note, we just loop until we find the first note
					
					current_pattern.Reset();
				}
				
				_patterns[GetCurrentPatternIndex()].Run();
				break;
			}
			
			case State::Play:
			{
				_patterns[GetCurrentPatternIndex()].SetOffset(0);
				_patterns[GetCurrentPatternIndex()].Run();
				break;
			}

			case State::Record:
			{
				
				Note note{};
				if (Platform::MidiNote::Read(_midi_interface, note))
				{
					const TimePoint now = Clock::now();
					const float interval = std::chrono::duration_cast<Seconds>(now - _last_note).count();
					
					_last_note = now;
					
					_patterns[GetCurrentPatternIndex()].AddNote(interval, note);
				}
			}

			default:
				_patterns[GetCurrentPatternIndex()].Stop();
				break;
		}
	}

	void Sequencer::SetBankIndex(unsigned int index)
	{
		if (index >= _pattern_index.size())
			throw std::runtime_error{ "Bank Index out of range" };

		_bank_index = index;
		ResizePatternContainer();
	}

	void Sequencer::SetPatternIndex(unsigned int index)
	{
		_pattern_index[_bank_index] = index;
		ResizePatternContainer();
	}

	void Sequencer::SetState(Sequencer::State state)
	{ 
		_state = state;

		switch (_state)
		{
		case State::Record:
			_last_note = Clock::now();
			break;
			
		default:
			break;
		}
		
	}
	
	void Sequencer::ResizePatternContainer()
	{
		while (_patterns.size() <= GetCurrentPatternIndex())
			_patterns.emplace_back(_midi_interface);
	}

	unsigned int Sequencer::GetCurrentPatternIndex() const
	{ return _pattern_index[_bank_index] * _pattern_index.size() + _bank_index; }
}
