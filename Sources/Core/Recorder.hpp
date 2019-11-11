#pragma once

#include <memory>
#include <vector>

#include "Core/Clock.hpp"
#include "Core/Note.hpp"
#include "Core/SequencerDef.hpp"
#include "Device/Device.hpp"

namespace Core {
	class Recorder {
	public:
		Recorder(std::shared_ptr<InputDevice> device);

		void Start(Pattern& pattern, unsigned int step_count);
		void Run();
		bool IsRecording() const;

	private:
		std::shared_ptr<InputDevice> _device;

		Pattern* _pattern;
		unsigned int _step_count{ 0 };
	};
}