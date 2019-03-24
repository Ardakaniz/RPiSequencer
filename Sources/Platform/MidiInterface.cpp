#include "Platform/MidiInterface.hpp"

#include <algorithm>

namespace Platform
{		
	MidiInterface::MidiInterface()
	{
		std::vector<std::string> in_ports{};
		for (unsigned int i{ 0 }; i < _in->getPortCount(); ++i)
			in_ports.push_back(_in->getPortName(i));
			
		for (unsigned int i{ 0 }; i < _out->getPortCount(); ++i)
		{
			const std::string port_name{ _out->getPortName(i) };
			
			auto in_it = std::find(std::begin(in_ports), std::end(in_ports), port_name);
			if (in_it != std::end(in_ports))
				_interfaces_id.emplace_back(std::distance(std::begin(in_ports), in_it), i);
		}
	}
	
	std::vector<std::string> MidiInterface::GetInterfacesName() const
	{
		std::vector<std::string> interfaces_name{};
		
		for (const auto& ids : _interfaces_id)
			interfaces_name.push_back(_in->getPortName(ids.first));
			
		return interfaces_name;
	}
	
	void MidiInterface::SelectInterface(unsigned int index)
	{
		_selected_interface = index;
		
		_in->openPort(_interfaces_id[_selected_interface].first);
		_in->ignoreTypes(false, false, false); //< what does that?
		
		_out->openPort(_interfaces_id[_selected_interface].second);
	}
	
	void MidiInterface::Poll()
	{
		std::vector<unsigned char> message{};
		_in->getMessage(&message);
		
		if (message.empty())
			return;
			
		_last_message = std::string{ std::begin(message), std::end(message) };
	}
	
	std::string MidiInterface::GetLastMessage()
	{ 
		std::string last_message{ _last_message };
		_last_message.clear();

		return last_message;
	}
	
	void MidiInterface::SendMessage(const std::string& message)
	{ SendMessage(std::vector<unsigned char>(std::begin(message), std::end(message))); }
	
	void MidiInterface::SendMessage(const std::vector<unsigned char>& message)
	{ _out->sendMessage(&message); }
}
