#include "Controller/Controller.hpp"

void Controller::OnNewEventCallback(EventCallback callback) {
	_on_event_callback = callback;
}

void Controller::Call(const Event& event) const {
	if (_on_event_callback.has_value())
		_on_event_callback.value()(event);
}