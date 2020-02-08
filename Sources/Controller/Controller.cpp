#include "Controller/Controller.hpp"

#include <cassert>

Controller::Controller(const std::vector<std::shared_ptr<SequenceGenerator>>& generators, const std::vector<std::shared_ptr<OutputDevice>>& output_devices) :
	_generators{ generators },
	_output_devices{ output_devices }
{ }

void Controller::OnNewEventCallback(EventCallback callback) {
	_on_event_callback = callback;
}

void Controller::OnRequestCallback(RequestCallback callback) {
	_on_request_callback = callback;
}

bool Controller::IsStopped() const {
	return _is_stopped;
}

bool Controller::NewEvent(const Event& event) const {
	if (_on_event_callback)
		return _on_event_callback.value()(event);

	return false; // The event has been ignored
}

Controller::Request::Data Controller::NewRequest(const Request& req) const {
	assert(_on_request_callback);

	return _on_request_callback.value()(req);
}