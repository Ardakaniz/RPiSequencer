#include "Core/Sequencer.hpp"

namespace Core {
	Sequencer::Sequencer(Controller& controller) :
		_controller{ controller }
	{
		_controller.OnNewEventCallback([this](const Controller::Event& event) {
			switch (event.type) {
			case Controller::Event::INPUT_DEVICE:
				_recorder.SetDevice(event.data.id);
				break;
			case Controller::Event::OUTPUT_DEVICE_ADD:
				_player.AddDevice(event.data.od);
				break;
			case Controller::Event::OUTPUT_DEVICE_REMOVE:
				_player.RemoveDevice(event.data.od);
				break;
			case Controller::Event::SEQUENCER_MODE:
				SetMode(event.data.i);
				break;
			case Controller::Event::PATTERN:
				return UpdatePatternIndex(_bank_index, event.data.i);
				break;
			case Controller::Event::BANK:
				return UpdatePatternIndex(event.data.i, _pattern_index);
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

	void Sequencer::SetMode(unsigned int flag) {
		_player.Stop();
		_recorder.Stop();

		switch (flag & 0b111) {
		case SeqMode_Record:
		{
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
	}

	bool Sequencer::UpdatePatternIndex(unsigned int bank, unsigned int pattern) {
		if (_recorder.IsRecording())
			return false; // If we are recording, we dont allow to change pattern selection

		_bank_index = bank;
		_pattern_index = pattern;
		return true;
	}
}