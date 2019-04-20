#pragma once

#ifndef CORE_SEQUENCER_HPP
#define CORE_SEQUENCER_HPP

#include "Core/Pattern.hpp"
#include "Core/Timer.hpp"
#include "Platform/MidiInterface.hpp"

#include <array>
#include <chrono>
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
		
		std::vector<std::string> GetInterfacesName();
		void SelectInterface(unsigned int id);
		void CloseInterface();

		void OnTap();
		void Run();

		void SetBankIndex(unsigned int index);
		void SetPatternIndex(unsigned int index);
		void SetState(State state);

	private:
		void ResizePatternContainer();
		unsigned int GetCurrentPatternIndex() const;

		Platform::MidiInterface _midi_interface;
		bool _no_valid_interface{ true };
		State _state{ State::Stop }; // Start stopped by default
		std::vector<Note> _notes_on;

		float _rate{ 1.f }; // Step once every seconds by default
		TimePoint _last_tap{};
		TimePoint _last_note{};

		unsigned int _bank_index{ 0 };
		std::array<unsigned int, 5> _pattern_index{ 0, 0, 0, 0, 0 };
		std::vector<Pattern> _patterns{};
	};
}

#endif // CORE_SEQUENCER_HPP
