#pragma once

#ifndef PLATFORM_MIDINOTE_HPP
#define PLATFORM_MIDINOTE_HPP

#include <string>

namespace Core
{ class Note; }

namespace Platform
{
	class MidiInterface;
	
	class MidiNote
	{
	public:
		static bool Read(MidiInterface& interface, Core::Note& note, bool get_duration = true);
		static void Trigger(MidiInterface& interface, const Core::Note& note);
		static void On(MidiInterface& interface, const Core::Note& note);
		static void Off(MidiInterface& interface, const Core::Note& note);

	private:
		enum class Message
		{
			On,
			Off,
			Invalid
		};

		static Message GetMessageType(const std::string& message);
	};
}

#endif // PLATFORM_MIDINOTE_HPP
