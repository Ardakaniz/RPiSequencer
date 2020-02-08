#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <optional>

#include "Core/Clock.hpp"
#include "Core/Pattern.hpp"
#include "Device/Device.hpp"

namespace Core {
	class Player {
	public:
		Player() = default;

		void AddDevice(std::shared_ptr<OutputDevice> device);
		void RemoveDevice(std::shared_ptr<OutputDevice> device);

		void Start(bool loop = true);
		void Start(Sequence& sequence, bool loop = true);
		void Restart();
		void EnableStepperMode(bool enable = true);
		void Stop();
		void Run();
		bool IsPlaying() const;

		std::size_t GetNoteIndexPlaying() const;

	private:
		void PlayCurrentNote(TimePoint now);
		void StopNote(const Note& note);

		std::vector<std::shared_ptr<OutputDevice>> _devices;

		std::optional<std::reference_wrapper<Sequence>> _sequence{ std::nullopt };
		std::vector<std::pair<Note, TimePoint>> _playing_note;
		TimePoint _change_note_instant;
		std::size_t _note_index{ 0 };

		bool _stopped{ true };
		bool _loop{ false }, _stepper_mode{ false };
		float _stepper_tempo{ 1.f }; // Note per minute
	};
}
