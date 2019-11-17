#include "Controller/Controller.hpp"

Controller::Controller(const std::vector<std::shared_ptr<InputDevice>>& input_devices, const std::vector<std::shared_ptr<OutputDevice>>& output_devices) :
	_input_devices{ input_devices },
	_output_devices{ output_devices }
{ }

void Controller::OnNewEventCallback(EventCallback callback) {
	_on_event_callback = callback;
}

bool Controller::IsStopped() const {
	return _is_stopped;
}

bool Controller::Call(const Event& event) const {
	if (_on_event_callback)
		return _on_event_callback.value()(event);

	return false; // The event has been ignored
}