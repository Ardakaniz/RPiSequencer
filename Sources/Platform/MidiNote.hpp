#pragma once

#ifndef PLATFORM_MIDINOTE_HPP
#define PLATFORM_MIDINOTE_HPP

namespace Core
{ class Note; }

namespace Platform
{
	class MidiInterface;
	
	class MidiNote
	{
	public:
		static bool Read(MidiInterface& interface, Core::Note& note);
		static void Trigger(MidiInterface& interface, const Core::Note& note);
		static void On(MidiInterface& interface, const Core::Note& note);
		static void Off(MidiInterface& interface, const Core::Note& note);
	};
}

#endif // PLATFORM_MIDINOTE_HPP
