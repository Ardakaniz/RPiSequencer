#pragma once

#include "Device/Device.hpp"

class LogDevice : public OutputDevice {
public:
	LogDevice() = default;

	void Open(unsigned int index) override;
	void Close() override;

	std::string GetDeviceType() const override;
	std::string GetDeviceName() const override;
	std::vector<std::string> GetDeviceList() override;

	void PlayNote(const Core::Note& note) override;
	void StopNote(const Core::Note& note) override;

private:
	std::ostream* _os{ nullptr };
	unsigned int _index{ 3 }; // 3 is an invalid index
};