#include "Core/Sequencer.hpp"

namespace Core {
	Sequencer::Sequencer(Controller& controller) :
		_controller{ controller }
	{
		_controller.OnNewEventCallback([this](const Controller::Event& event) {
			switch (event.type) {
			case Controller::Event::INPUT_DEVICE:
				_recorder.SetDevice(std::get<std::shared_ptr<InputDevice>>(event.data));
				_can_record = true;
				break;
			case Controller::Event::OUTPUT_DEVICE_ADD:
				_player.AddDevice(std::get<std::shared_ptr<OutputDevice>>(event.data));
				break;
			case Controller::Event::OUTPUT_DEVICE_REMOVE:
				_player.RemoveDevice(std::get<std::shared_ptr<OutputDevice>>(event.data));
				break;
			case Controller::Event::SEQUENCER_MODE:
				return SetMode(std::get<unsigned int>(event.data));
				break;
			case Controller::Event::PATTERN:
				return UpdatePatternIndex(_bank_index, std::get<unsigned int>(event.data));
				break;
			case Controller::Event::BANK:
				return UpdatePatternIndex(std::get<unsigned int>(event.data), _pattern_index);
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

	void Sequencer::Run() {
		_controller.Run();
		_recorder.Run();
		_player.Run();
	}

	bool Sequencer::SetMode(unsigned int flag) {
		_player.Stop();
		_recorder.Stop();

		switch (flag & 0b111) {
		case SeqMode_Record:
		{
			if (!_can_record)
				return false;

			unsigned int step_count = 0;
			if (_pattern_index > 0)
				step_count = static_cast<unsigned int>(GetPattern(0).first.size());

			_recorder.Start(GetPattern(), step_count);

			break;
		}
		case SeqMode_Play:
		{
			_player.Start(GetPattern());
			break;
		}
		}

		return true;
	}

	bool Sequencer::UpdatePatternIndex(unsigned int bank, unsigned int pattern) {
		if (_recorder.IsRecording())
			return false; // If we are recording, we dont allow to change pattern selection

		_bank_index = bank;
		_pattern_index = pattern;
		return true;
	}
}