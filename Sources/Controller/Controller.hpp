#pragma once

#include <functional>

class Controller {
public:
	struct Event {
		enum Type {
			INPUT_DEVICE,
			OUTPUT_DEVICE,
			SEQUENCER_MODE,
			PATTERN,
			BANK,
			TAP,
		} type;

		std::string s;
		bool b;
		unsigned int i;
	};

	using EventCallback = std::function<void(const Event&)>;

	Controller() = default;
	virtual ~Controller() = default;

	virtual void OnNewEventSig(EventCallback callback);

	virtual void Poll() = 0;

private:
	EventCallback _on_event_callback;
};