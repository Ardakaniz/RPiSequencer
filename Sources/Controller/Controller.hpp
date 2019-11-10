#pragma once

#include <functional>
#include <optional>

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

	virtual void OnNewEventCallback(EventCallback callback);

	virtual void Poll() = 0;

protected:
	void Call(const Event& event) const;

private:
	std::optional<EventCallback> _on_event_callback{ std::nullopt };
};