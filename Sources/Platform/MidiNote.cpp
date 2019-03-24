#include "Platform/MidiNote.hpp"

#include "Core/Note.hpp"
#include "Platform/MidiInterface.hpp"

#include <chrono>
#include <thread>

namespace Platform
{
	bool MidiNote::Read(MidiInterface& interface, Core::Note& note)
	{		
		std::string msg{ interface.GetLastMessage() };
		if (msg.length() != 3)
			return false;
			
		if (msg[0] == static_cast<unsigned char>(144))
		{
			note._is_blank_note = false;
			note._note = static_cast<unsigned int>(msg[1]);
			return true;
		}
				
		return false;
	}
		
	void MidiNote::Trigger(MidiInterface& interface, const Core::Note& note)
	{
		using namespace std::chrono_literals;
		
		if (note._is_blank_note)
			return;
		
		On(interface, note);
		std::this_thread::sleep_for(10ms); // We hold the note for the shortest time possible to control that on the synth
		Off(interface, note);
	}

	void MidiNote::On(MidiInterface& interface, const Core::Note& note)
	{
		if (note._is_blank_note)
			return;
		
		std::vector<unsigned char> msg{ static_cast<unsigned char>(144), static_cast<unsigned char>(note._note), static_cast<unsigned char>(69) };
		interface.SendMessage(msg); // Note On
	}

	void MidiNote::Off(MidiInterface& interface, const Core::Note& note)
	{
		std::vector<unsigned char> msg{ static_cast<unsigned char>(128), static_cast<unsigned char>(note._note), static_cast<unsigned char>(69) };
		interface.SendMessage(msg); // Note Off
	}
}
