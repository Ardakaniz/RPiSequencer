#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

class InputDevice;
class OutputDevice;

class Controller {
public:
	struct Event {
		enum Type {
			INPUT_DEVICE,
			OUTPUT_DEVICE_ADD,
			OUTPUT_DEVICE_REMOVE,
			SEQUENCER_MODE,
			PATTERN,
			BANK,
			TAP,
		} type;

		std::variant<bool, unsigned int, std::string, std::shared_ptr<InputDevice>, std::shared_ptr<OutputDevice>> data;
	};

	using EventCallback = std::function<bool(const Event&)>; // Return false if the event hasnt been accepted or ignored

	Controller(const std::vector<std::shared_ptr<InputDevice>>& input_devices, const std::vector<std::shared_ptr<OutputDevice>>& output_devices);
	virtual ~Controller() = default;

	virtual void OnNewEventCallback(EventCallback callback);

	virtual void Run() = 0;

protected:
	bool Call(const Event& event) const;

	const std::vector<std::shared_ptr<InputDevice>> _input_devices;
	const std::vector<std::shared_ptr<OutputDevice>> _output_devices;

private:
	std::optional<EventCallback> _on_event_callback{ std::nullopt };
};