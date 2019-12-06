#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <optional>

#include "Core/Clock.hpp"
#include "Core/Note.hpp"
#include "Core/SequencerDef.hpp"
#include "Device/Device.hpp"

namespace Core {
	class Recorder {
	public:
		Recorder() = default;

		void SetDevice(std::shared_ptr<InputDevice> device);

		bool Start(Pattern& pattern, unsigned int step_count);
		void Stop();
		void Run();
		bool IsRecording() const;

	private:
		std::shared_ptr<InputDevice> _device;

		std::optional<std::reference_wrapper<Pattern>> _pattern{ std::nullopt };
		unsigned int _step_count{ 0 };
		std::vector<std::size_t> _pressed_notes_id;
		std::optional<TimePoint> _last_release_point{ std::nullopt };
		std::optional<Duration> _append_offset{ std::nullopt };
	};
}