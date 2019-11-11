#include "Core/Recorder.hpp"

namespace Core {
	Recorder::Recorder(std::shared_ptr<InputDevice> device) :
		_device{ device }
	{ }

	void Recorder::Start(Pattern& pattern, unsigned int step_count) {
		_pattern = pattern;
		_step_count = step_count;

		_pattern->get().second = Clock::now();
	}

	void Recorder::Stop() {
		_pattern = std::nullopt;
	}

	void Recorder::Run() {
		if (!IsRecording())
			return;

		std::optional<std::pair<Core::Note, bool /* is_pressed? */>> data = _device->ReadNote();
		if (!data)
			return;

		auto note = data->first;
		if (data->second)
			note.pressed_instant = Clock::now();
		else
			note.release_instant = Clock::now();

		_pattern->get().first.push_back(note);
	}

	bool Recorder::IsRecording() const { 
		return (_device) && (_pattern) && (_step_count == 0 || _pattern->get().first.size() < _step_count); // If the pattern isn't filled yet
	}
}