#include "Core/Recorder.hpp"

namespace Core {
	Recorder::Recorder(std::shared_ptr<InputDevice> device) :
		_device{ device }
	{ }

	void Recorder::RecordStep(std::vector<Note>& pattern, unsigned int step_count) {
		if (step_count > 0 && pattern.size() >= step_count)
			return;
	}
}