#include "Core/Sequencer.hpp"

#include <stdexcept>

namespace Core
{
	Sequencer::Sequencer() :
		_last_step{ Clock::now() },
		_last_tap{ Clock::now() }
	{
		_patterns.emplace_back(); // Create one first blank pattern so that _pattern_index is not out of bound
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
			case State::Transpose:
			{
				// TODO: Manage MIDI Inputs
				break;
			}

			case State::Play:
			{
				if (step)
					_patterns[GetCurrentPatternIndex()].Step();
				break;
			}

			case State::Record:
			{
				// TODO: Manage MIDI Inputs
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
	}

	void Sequencer::SetPatternIndex(unsigned int index)
	{
		_pattern_index[_bank_index] = index;

		if (_patterns.size() <= index)
			_patterns.resize(index + 1);
	}

	void Sequencer::SetRate(float rate)
	{ _rate = rate; }

	void Sequencer::SetState(Sequencer::State state)
	{ 
		_state = state;

		if (_state == State::Play)
			_last_step = Clock::now();
	}

	void Sequencer::SetStepCallback(const std::function<void()>& callback)
	{ _step_callback = callback; }

	float Sequencer::GetRate() const
	{ return _rate; }

	unsigned int Sequencer::GetCurrentPatternIndex() const
	{ return _pattern_index[_bank_index] * _pattern_index.size() + _bank_index; }
}
