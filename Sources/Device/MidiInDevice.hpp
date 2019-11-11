#pragma once

#include "Device/Device.hpp"
#include "RtMidi/RtMidi.hpp"

#include <memory>

class MidiInDevice : public InputDevice {
public:
	void Open(unsigned int index) override;
	void Close() override;

	std::string GetDeviceType() const override;
	std::string GetDeviceName() const override;
	std::vector<std::string> GetDeviceList() override;

	std::optional<std::pair<Core::Note, bool /*pressed?*/>> ReadNote() override;

private:
	std::unique_ptr<RtMidiIn> _midi{ std::make_unique<RtMidiIn>() };
	unsigned int _index{ 0 };
};