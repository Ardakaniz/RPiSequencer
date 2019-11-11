#pragma once

#include <functional>
#include <optional>

/*
TODO: Prendre des refs sur les devices pour pouvoir ouvrir/fermer/choisir les ports depuis le controleur
*/

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

	using EventCallback = std::function<bool(const Event&)>; // Return false if the event hasnt been accepted or ignored

	Controller() = default;
	virtual ~Controller() = default;

	virtual void OnNewEventCallback(EventCallback callback);

	virtual void Poll() = 0;

protected:
	bool Call(const Event& event) const;

private:
	std::optional<EventCallback> _on_event_callback{ std::nullopt };
};