#pragma once

#ifndef CORE_SEQUENCER_HPP
#define CORE_SEQUENCER_HPP

#include "Core/Pattern.hpp"
#include "Platform/MidiInterface.hpp"

#include <array>
#include <chrono>
#include <functional>
#include <vector>

namespace Core
{
	class Sequencer
	{
	public:
		enum class State
		{
			Transpose,
			Play, 
			Record,
			Stop
		};

		Sequencer();

		void OnTap();
		void Run();

		void SetBankIndex(unsigned int index);
		void SetPatternIndex(unsigned int index);
		void SetRate(float rate);
		void SetState(State state);
		void SetStepCallback(const std::function<void()>& callback);

		float GetRate() const;

	private:
		using Clock = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		using Seconds = std::chrono::duration<float>; // Because std::chrono::seconds gives integer and the rate is a floating point

		unsigned int GetCurrentPatternIndex() const;

		Platform::MidiInterface _midi_interface;
		State _state{ State::Stop }; // Start stopped by default

		float _rate{ 1.f }; // Step once every seconds by default
		std::function<void()> _step_callback{ [](){} };
		TimePoint _last_step{};
		TimePoint _last_tap{};

		unsigned int _bank_index{ 0 };
		std::array<unsigned int, 5> _pattern_index{ 0, 0, 0, 0, 0 };
		std::vector<Pattern> _patterns{};
	};
}

#endif // CORE_SEQUENCER_HPP
