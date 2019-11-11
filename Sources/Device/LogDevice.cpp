#include "Device/LogDevice.hpp"

#include <iostream>
#include <string>

void LogDevice::Open(unsigned int index) {
	switch (index) {
	case 0:
		_os = &std::cout;
		break;

	case 1:
		_os = &std::cerr;
		break;

	default:
		throw std::runtime_error{ "[LogDevice] Invalid index (" + std::to_string(index) + ")" };
		break;
	}

	_index = index;
}

void LogDevice::Close()
{ _os = nullptr; }

std::string LogDevice::GetDeviceType() const
{ return "Log Device"; }

std::string LogDevice::GetDeviceName() const
{
	switch (_index) {
	case 0:
		return "Standard Output";

	case 1:
		return "Error Output";

	default:
		throw std::runtime_error{ "[LogDevice] Cannot get device name: Invalid device" };
	}
}

std::vector<std::string> LogDevice::GetDeviceList() {
	return { "Standard Output", "Error Output" };
}

void LogDevice::PlayNote(const Core::Note& note) {
	if (_os == nullptr)
		throw std::runtime_error{ "[LogDevice] Trying to use invalid device" };

	(*_os) << "Playing Note " << note.note << " for " << std::chrono::duration_cast<Core::Duration>(note.release_instant - note.pressed_instant).count() / (1'000'000'000.f) << "s" << std::endl;
}

void LogDevice::StopNote(const Core::Note& note) {
	if (_os == nullptr)
		throw std::runtime_error{ "[LogDevice] Trying to use invalid device" };

	(*_os) << "Stopped Note " << note.note << std::endl;
}