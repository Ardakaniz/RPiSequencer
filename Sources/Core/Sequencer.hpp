#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Controller/Controller.hpp"
#include "Device/Device.hpp"
#include "Core/Note.hpp"
#include "Core/Player.hpp"
#include "Core/Recorder.hpp"
#include "Core/SequencerDef.hpp"

namespace Core {
	class Sequencer {
	public:
		Sequencer(Controller& controller, std::shared_ptr<InputDevice> device_in, const std::vector<std::shared_ptr<OutputDevice>>& devices_out);

		void SetMode(SeqMode mode, bool stepper = false);

		void Run();

	private:
		bool UpdatePatternIndex(unsigned int bank, unsigned int pattern);

		inline const Pattern& GetPattern() const;
		inline Pattern& GetPattern();
		inline const Pattern& GetPattern(unsigned int pattern_index, unsigned int bank_index = BANK_COUNT) const;
		inline Pattern& GetPattern(unsigned int pattern_index, unsigned int bank_index = BANK_COUNT);

		Controller& _controller;
		Recorder _recorder;
		Player _player;

		std::array<Pattern, BANK_COUNT * PATTERN_COUNT> _notes{};
		unsigned int _pattern_index{ 0 }, _bank_index{ 0 };

		SeqMode _mode{ SeqMode::Stop };
		bool _stepper_mode{ false };
	};
}

#include "Core/Sequencer.inl"