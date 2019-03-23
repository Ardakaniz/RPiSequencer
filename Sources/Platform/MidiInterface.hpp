#pragma once

#ifndef PLATFORM_MIDIINTERFACE_HPP
#define PLATFORM_MIDIINTERFACE_HPP

#include <RtMidi.h>

#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>

namespace Platform
{
	class MidiInterface // MidiInterface = MidiIn + MidiOut
	{
	public:
		MidiInterface();
		
		std::vector<std::string> GetInterfacesName() const;
		void SelectInterface(unsigned int index);
		
		void Poll();
		std::string PopMessage();
		void SendMessage(const std::string& message);
		void SendMessage(const std::vector<unsigned char>& message);
		
	private:
		std::unique_ptr<RtMidiIn> _in{ std::make_unique<RtMidiIn>() };
		std::unique_ptr<RtMidiOut> _out{ std::make_unique<RtMidiOut>() };
		std::stack<std::string> _in_messages{};
		unsigned int _selected_interface;
		std::vector<std::pair<unsigned int, unsigned int>> _interfaces_id; // (in, out)
	};
}

#endif // PLATFORM_MIDIINTERFACE_HPP
