#pragma once

#include <functional>
#include <optional>

#include "Core/Note.hpp"

class Device {
public:
	Device() = default;
	virtual ~Device();

	virtual void Open(unsigned int index) = 0;
	virtual void Close() = 0;

	virtual std::string GetDeviceType() const = 0;
	virtual std::string GetDeviceName() const = 0;
	virtual std::vector<std::string> GetDeviceList() = 0;
};

class InputDevice : Device {
public:
	InputDevice() = default;
	virtual ~InputDevice() = default;

	virtual std::optional<Core::Note> ReadNote() = 0;
};

class OutputDevice : Device {
public:
	OutputDevice() = default;
	virtual ~OutputDevice() = default;

	virtual void SendNote(const Core::Note& note) = 0;
};