#include "Core/Sequencer.hpp"

namespace Core {
	Sequencer::Sequencer(Controller& controller, std::shared_ptr<InputDevice> device_in, const std::vector<std::shared_ptr<OutputDevice>>& devices_out) :
		_controller{ controller },
		_recorder{ device_in },
		_player{ devices_out }
	{

	}

	void Sequencer::SetMode(SeqMode mode, bool stepper) {
		_mode = mode;
		_stepper_mode = stepper;
	}

	void Sequencer::Run() {
		switch (_mode) {
		case SeqMode::Record:
		{
			unsigned int step_count = 0;
			if (_pattern_index > 0)
				step_count = static_cast<unsigned int>(GetPattern(0).first.size());

			_recorder.Start(GetPattern(), step_count);
			
			break;
		}
		}
		}

		_recorder.Run();
	}
}