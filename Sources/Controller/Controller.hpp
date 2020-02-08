#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

class InputDevice;
class OutputDevice;
class SequenceGenerator;

class Controller {
public:
	struct Event {
		enum Type {
			UPDATE_GENERATOR,
			ADD_OUTPUT_DEVICE,
			REMOVE_OUTPUT_DEVICE,

			PLAY,
			TRANSPOSE,
			GENERATE,
			STOP_GENERATION,
			STOP,

			SELECT_PATTERN,
			SELECT_BANK,
			TAP,
		} type;

		std::variant<unsigned int,std::shared_ptr<InputDevice>, std::shared_ptr<OutputDevice>, std::shared_ptr<SequenceGenerator>> data;
	};

	struct Request {
		enum Type {
			SEQUENCE_SIZE,
			SEQUENCE_STEP_INDEX,
			SEQUENCE_STEP_MUTE,

			IS_GENERATING,
		} type;

		using Data = std::variant<std::size_t, bool>;
	};

	using EventCallback   = std::function<bool(const Event&)>; // Return false if the event hasnt been accepted or ignored
	using RequestCallback = std::function<const Request::Data& (const Request&)>;

	Controller(const std::vector<std::shared_ptr<SequenceGenerator>>& generators, const std::vector<std::shared_ptr<OutputDevice>>& output_devices);
	virtual ~Controller() = default;

	virtual void OnNewEventCallback(EventCallback callback);
	virtual void OnRequestCallback(RequestCallback callback);
	bool IsStopped() const;

	virtual void Run() = 0;

protected:
	bool NewEvent(const Event& event) const;
	Request::Data NewRequest(const Request& req) const;

	bool _is_stopped{ false };

	const std::vector<std::shared_ptr<SequenceGenerator>> _generators;
	const std::vector<std::shared_ptr<OutputDevice>> _output_devices;

private:
	std::optional<EventCallback> _on_event_callback{ std::nullopt };
	std::optional<RequestCallback> _on_request_callback{ std::nullopt };
};