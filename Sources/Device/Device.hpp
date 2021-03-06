#pragma once

#include <functional>
#include <optional>

#include "Core/Note.hpp"

class Device {
public:
	Device() = default;
	virtual ~Device() = default;

	virtual void Open(unsigned int index) = 0;
	virtual bool IsOpen() const = 0;
	virtual void Close() = 0;

	virtual std::string GetDeviceType() const = 0;
	virtual std::string GetDeviceName() const = 0;
	virtual std::vector<std::string> GetDeviceList() = 0;
};

class InputDevice : public Device {
public:
	InputDevice() = default;
	virtual ~InputDevice() = default;

	virtual std::optional<std::pair<Core::Note, bool /*pressed?*/>> ReadNote() = 0;
};

class OutputDevice : public Device {
public:
	OutputDevice() = default;
	virtual ~OutputDevice() = default;

	virtual void PlayNote(const Core::Note& note) = 0;
	virtual void StopNote(const Core::Note& note) = 0;
};