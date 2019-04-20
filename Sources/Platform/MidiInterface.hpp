#pragma once

#ifndef PLATFORM_MIDIINTERFACE_HPP
#define PLATFORM_MIDIINTERFACE_HPP

#include <RtMidi.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Platform
{
	class MidiInterface // MidiInterface = MidiIn + MidiOut
	{
		friend class MidiNote;
		
	public:
		MidiInterface();
		
		bool UpdateInterfaces();
		std::vector<std::string> GetInterfacesName() const;
		void SelectInterface(unsigned int index);
		void CloseInterface();
		
		void Poll();
		
	private:
		std::string GetLastMessage();
		void SendMessage(const std::string& message);
		void SendMessage(const std::vector<unsigned char>& message);
	
		std::unique_ptr<RtMidiIn> _in{ std::make_unique<RtMidiIn>() };
		std::unique_ptr<RtMidiOut> _out{ std::make_unique<RtMidiOut>() };
		std::string _last_message{};
		unsigned int _selected_interface;
		std::vector<std::pair<unsigned int, unsigned int>> _interfaces_id; // (in, out)
	};
}

#endif // PLATFORM_MIDIINTERFACE_HPP
