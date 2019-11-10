#pragma once

#include <memory>
#include <vector>

#include "Core/Note.hpp"
#include "Device/Device.hpp"

namespace Core {
	class Recorder {
	public:
		Recorder(std::shared_ptr<InputDevice> device);

		void RecordStep(std::vector<Note>& pattern, unsigned int step_count);

	private:
		std::shared_ptr<InputDevice> _device;
	};
}