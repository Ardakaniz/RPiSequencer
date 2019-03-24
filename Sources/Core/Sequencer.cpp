#include "Core/Sequencer.hpp"

#include <stdexcept>
#include <iostream>

namespace Core
{
	Sequencer::Sequencer() :
		_last_step{ Clock::now() },
		_last_tap{ Clock::now() }
	{
		_patterns.emplace_back(); // Create one first blank pattern so that _pattern_index is not out of bound
		
		// TMP
		const std::vector<std::string>& interfaces{ _midi_interface.GetInterfacesName() };
		for (unsigned int i{ 0 }; i < interfaces.size(); ++i)
			std::cout << "Interface #" << i << ": " << interfaces[i] << std::endl;
		
		std::cout << "> ";
		unsigned int id;
		std::cin >> id;
		_midi_interface.SelectInterface(id);
	}

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

			case State::Record: // We add a blank note
					_patterns[GetCurrentPatternIndex()].AddNote();
				break;
		}
	}

	void Sequencer::Run()
	{
		_midi_interface.Poll();
		
		bool step{ false };
		const TimePoint now = Clock::now();
		const float since_last_step = std::chrono::duration_cast<Seconds>(now - _last_step).count();

		if (since_last_step >= _rate)
		{
			_step_callback();
			step = true;
			_last_step = now;
		}

		switch (_state)
		{
			case State::Transpose: // We change the transpose offset when a note is triggered and then we fall back to State::Play 
			{
				Note note{};
				if (Platform::MidiNote::Read(_midi_interface, note))
				{
					Pattern& current_pattern{ _patterns[GetCurrentPatternIndex()] };
					
					current_pattern.Reset();
					while (true)
					{
						try
						{ 
							_transpose_offset = current_pattern.GetNote().Offset(note);
						}
						catch (const std::runtime_error&) // The current note is blank, we step the pattern and compute the offset with the next note
						{
							current_pattern.Step();
							continue;
						}

						break;
					}
					current_pattern.Reset();
				}
			}

			case State::Play:
			{
				if (step && !_patterns[GetCurrentPatternIndex()].IsEmpty())
				{
					Platform::MidiNote::Trigger(_midi_interface, Note{ _patterns[GetCurrentPatternIndex()].GetNote(), _transpose_offset });
					_patterns[GetCurrentPatternIndex()].Step();
				}
				break;
			}

			case State::Record:
			{
				Note note{};
				if (Platform::MidiNote::Read(_midi_interface, note))
					_patterns[GetCurrentPatternIndex()].AddNote(note);
					
				break;
			}

			default: // = Stop = we dont do anything
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

	void Sequencer::SetRate(float rate)
	{ _rate = rate; }

	void Sequencer::SetState(Sequencer::State state)
	{ 
		_state = state;

		if (_state == State::Play)
		{
			_last_step = Clock::now();
			_transpose_offset = 0;
		}
	}

	void Sequencer::SetStepCallback(const std::function<void()>& callback)
	{ _step_callback = callback; }

	float Sequencer::GetRate() const
	{ return _rate; }
	
	void Sequencer::ResizePatternContainer()
	{
		if (_patterns.size() <= GetCurrentPatternIndex())
			_patterns.resize(GetCurrentPatternIndex() + 1);
	}

	unsigned int Sequencer::GetCurrentPatternIndex() const
	{ return _pattern_index[_bank_index] * _pattern_index.size() + _bank_index; }
}
