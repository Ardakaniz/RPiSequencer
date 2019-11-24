#pragma once

#include "Device/Device.hpp"
#include "RtMidi/RtMidi.hpp"

#include <memory>

class MidiOutDevice : public OutputDevice {
public:
	void Open(unsigned int index) override;
	bool IsOpen() const override;
	void Close() override;

	std::string GetDeviceType() const override;
	std::string GetDeviceName() const override;
	std::vector<std::string> GetDeviceList() override;

	void PlayNote(const Core::Note& note) override;
	void StopNote(const Core::Note& note) override;

private:
	std::unique_ptr<RtMidiOut> _midi{ std::make_unique<RtMidiOut>() };
	unsigned int _index{ 0 };
};