#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <optional>

#include "Core/Clock.hpp"
#include "Core/SequencerDef.hpp"
#include "Device/Device.hpp"

namespace Core {
	class Player {
	public:
		Player() = default;

		void AddDevice(std::shared_ptr<OutputDevice> device);
		void RemoveDevice(std::shared_ptr<OutputDevice> device);

		void Start(Pattern& pattern, bool loop = true);
		void EnableStepperMode(bool enable = true);
		void Stop();
		void Run();
		bool IsPlaying() const;

	private:
		void Start(); // Actually used to restart
		Note ShiftNoteTime(const Note& note);
		void PlayNote(const Note& note);
		void StopNote(const Note& note);

		std::vector<std::shared_ptr<OutputDevice>> _devices;

		std::optional<std::reference_wrapper<Pattern>> _pattern{ std::nullopt };
		TimePoint _start_point{};
		std::size_t _note_index{ 0 };
		std::vector<Note> _played_note{};
		bool _loop{ true }, _stepper_mode{ false };
		float _stepper_tempo{ 1.f }; // Note per minute
	};
}
