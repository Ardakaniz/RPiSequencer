#include "Core/Sequencer.hpp"

namespace Core {
	Sequencer::Sequencer(Controller& controller, std::shared_ptr<InputDevice> device_in, const std::vector<std::shared_ptr<OutputDevice>>& devices_out) :
		_controller{ controller },
		_recorder{ device_in },
		_player{ devices_out }
	{
		_controller.OnNewEventCallback([this](const Controller::Event& event) {
			switch (event.type) {
			case Controller::Event::INPUT_DEVICE:
				break;
			case Controller::Event::OUTPUT_DEVICE:
				break;
			case Controller::Event::SEQUENCER_MODE:
				SetMode(static_cast<SeqMode>(event.i), event.b);
				break;
			case Controller::Event::PATTERN:
				return UpdatePatternIndex(_bank_index, event.i);
				break;
			case Controller::Event::BANK:
				return UpdatePatternIndex(event.i, _pattern_index);
				break;
			case Controller::Event::TAP:
				break;
			}

			return true;
		});
	}

	Sequencer::~Sequencer() {
		_player.Stop();
		_recorder.Stop();
	}

	void Sequencer::SetMode(SeqMode mode, bool stepper) {
		_mode = mode;
		_stepper_mode = stepper;

		_player.Stop();
		_recorder.Stop();

		switch (_mode) {
		case SeqMode::Record:
		{
			unsigned int step_count = 0;
			if (_pattern_index > 0)
				step_count = static_cast<unsigned int>(GetPattern(0).first.size());

			_recorder.Start(GetPattern(), step_count);

			break;
		}
		case SeqMode::Play:
		{
			_player.Start(GetPattern());
			break;
		}
		}
	}

	void Sequencer::Run() {
		_controller.Run();
		_recorder.Run();
		_player.Run();
	}

	bool Sequencer::UpdatePatternIndex(unsigned int bank, unsigned int pattern) {
		if (_recorder.IsRecording())
			return false; // If we are recording, we dont allow to change pattern selection

		_bank_index = bank;
		_pattern_index = pattern;
		return true;
	}
}