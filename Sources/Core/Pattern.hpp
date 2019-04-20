#pragma once

#ifndef CORE_PATTERN_HPP
#define CORE_PATTERN_HPP

#include "Core/Note.hpp"
#include "Core/Timer.hpp"
#include "Platform/MidiInterface.hpp"

#include <vector>

namespace Core
{
	class Pattern
	{
	public:
		Pattern(Platform::MidiInterface& midi_interface);

		template<typename... Args> void AddNote(float interval, Args... args);
		void Clear();

		void Run();
		void Stop();
		void Reset();
		void SetOffset(int offset);
		const Note& GetNote() const;
		bool IsEmpty() const;

	private:
		void Step();

		Platform::MidiInterface& _midi_interface;
		TimePoint _last_step;
		int _offset { 0 };
		std::vector<Note> _notes;
		std::vector<std::pair<Note, TimePoint>> _notes_on; // Notes on can be different from the notes stored because of the offset
		std::vector<float> _intervals;
		unsigned int _step{ 0 };
	};
}

#include "Core/Pattern.inl"

#endif // CORE_PATTERN_HPP
