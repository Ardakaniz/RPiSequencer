#include "Controller/Controller.hpp"

void Controller::OnNewEventCallback(EventCallback callback) {
	_on_event_callback = callback;
}

bool Controller::Call(const Event& event) const {
	if (_on_event_callback)
		return _on_event_callback.value()(event);

	return false; // The event has been ignored
}