#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Controller/Controller.hpp"
#include "Device/Device.hpp"
#include "Core/Note.hpp"
#include "Core/Player.hpp"
#include "Core/Recorder.hpp"

namespace Core {
	class Sequencer {
	public:
		static constexpr const unsigned int BANK_COUNT = 100;
		static constexpr const unsigned int PATTERN_COUNT = 5;
		enum class Mode {
			Play,
			Transpose,
			Arpegiator,
			Record,
			Stop
		};

		using Pattern = std::pair<std::vector<Note>, TimePoint /* pattern_start */>;

		Sequencer(Controller& controller, std::shared_ptr<InputDevice> device_in, const std::vector<std::shared_ptr<OutputDevice>>& devices_out);

		void SetMode(Mode mode, bool stepper = false);

		void Run();

	private:
		inline const Pattern& GetPattern() const;
		inline Pattern& GetPattern();
		inline const Pattern& GetPattern(unsigned int pattern_index, unsigned int bank_index = BANK_COUNT) const;
		inline Pattern& GetPattern(unsigned int pattern_index, unsigned int bank_index = BANK_COUNT);

		Controller& _controller;
		Recorder _recorder;
		Player _player;

		std::array<Pattern, BANK_COUNT * PATTERN_COUNT> _notes;
		unsigned int _pattern_index{ 0 }, _bank_index{ 0 };

		Mode _mode{ Mode::Stop };
		bool _stepper_mode{ false };
	};
}

#include "Core/Sequencer.inl"