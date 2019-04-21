#include "Platform/MidiNote.hpp"

#include "Core/Note.hpp"
#include "Core/Timer.hpp"
#include "Platform/MidiInterface.hpp"

#include <chrono>
#include <thread>

namespace Platform
{
	bool MidiNote::Read(MidiInterface& interface, Core::Note& note, TimePoint& trigger_point, bool get_duration)
	{
		std::string msg{ interface.GetLastMessage() };
		switch (GetMessageType(msg))
		{
			case Message::On:
			{
				note._note = static_cast<unsigned int>(msg[1]);

				if (get_duration)
				{
					trigger_point = Clock::now();

					while (true) // We wait for the note to be released
					{
						interface.Poll();
						msg = interface.GetLastMessage();
						
						if (GetMessageType(msg) == Message::Off)
						{
							const TimePoint now = Clock::now();
							note._duration = std::chrono::duration_cast<Seconds>(now - trigger_point).count();
							
							break;
						}
					}
				}

				return true;
			}

			default:
				return false;
		}
	}
		
	void MidiNote::Trigger(MidiInterface& interface, const Core::Note& note)
	{
		using namespace std::chrono_literals;

		On(interface, note);
		std::this_thread::sleep_for(10ms); // We hold the note for the shortest time possible to control that on the synth
		Off(interface, note);
	}

	void MidiNote::On(MidiInterface& interface, const Core::Note& note)
	{
		std::vector<unsigned char> msg{ static_cast<unsigned char>(144), static_cast<unsigned char>(note._note), static_cast<unsigned char>(69) };
		interface.SendMessage(msg); // Note On
	}

	void MidiNote::Off(MidiInterface& interface, const Core::Note& note)
	{
		std::vector<unsigned char> msg{ static_cast<unsigned char>(128), static_cast<unsigned char>(note._note), static_cast<unsigned char>(69) };
		interface.SendMessage(msg); // Note Off
	}

	MidiNote::Message MidiNote::GetMessageType(const std::string& message)
	{
		if (message.length() != 3)
			return Message::Invalid;
		else if (message[0] == static_cast<unsigned char>(144))
			return Message::On;
		else if (message[0] == static_cast<unsigned char>(128))
			return Message::Off;
		else
			return Message::Invalid;
	}
}
