#include "Core/Sequencer.hpp"

#include "SequenceGenerator/SequenceGenerator.hpp"

namespace Core {
	Sequencer::Sequencer(Controller& controller) :
		_controller{ controller }
	{
		_controller.OnNewEventCallback([this](const Controller::Event& event) {
			switch (event.type) {
			case Controller::Event::UPDATE_GENERATOR:
				if (_generator)
					_generator->Stop();

				_generator = std::get<decltype(_generator)>(event.data);
				break;
			case Controller::Event::ADD_OUTPUT_DEVICE:
				_player.AddDevice(std::get<std::shared_ptr<OutputDevice>>(event.data));
				break;
			case Controller::Event::REMOVE_OUTPUT_DEVICE:
				_player.RemoveDevice(std::get<std::shared_ptr<OutputDevice>>(event.data));
				break;
			case Controller::Event::PLAY:
				if (_changed_sequence) {
					_player.Start(_patterns[_pattern_index].GetSequence());
					_changed_sequence = false;
				}
				else
					_player.Start();
				break;
			case Controller::Event::TRANSPOSE:
				break;
			case Controller::Event::GENERATE:
			{
				if (!_generator)
					return false;

				_generator->Start();
			}
				break;
			case Controller::Event::STOP_GENERATION:
				if (!_generator)
					return false;

				_generator->Stop();
				_patterns[_pattern_index].GetSequence().Append(*_generator); // Add final notes
				
				break;
			case Controller::Event::STOP:
				_player.Stop();
				break;
			case Controller::Event::SELECT_PATTERN:
				return ChangeSequence(_pattern_index, std::get<unsigned int>(event.data));
				break;
			case Controller::Event::SELECT_BANK:
				return ChangeSequence(std::get<unsigned int>(event.data));
				break;
			case Controller::Event::TAP:
				break;
			}

			return true;
		});

		_controller.OnRequestCallback([this](const Controller::Request& req) -> Controller::Request::Data {
			switch (req.type) {
			case Controller::Request::SEQUENCE_SIZE:
				return _patterns[_pattern_index].GetSequence().GetSize();

			case Controller::Request::SEQUENCE_STEP_INDEX:
				return _player.GetNoteIndexPlaying();

			case Controller::Request::SEQUENCE_STEP_MUTE:
				return false;

			case Controller::Request::IS_GENERATING:
				if (_generator)
					return !_generator->GenerationFinished();
				else
					return false;
			}
		});
	}

	Sequencer::~Sequencer() {
		_player.Stop();
		if (_generator)
			_generator->Stop();
	}

	void Sequencer::Run() {
		_controller.Run();
		_player.Run();

		if (_generator && !_generator->GenerationFinished()) {
			_generator->Run();
			_patterns[_pattern_index].GetSequence().Append(*_generator);
		}
	}

	bool Sequencer::ChangeSequence(std::size_t pattern, std::size_t sequence) {
		if (pattern >= BANK_SIZE || (sequence >= PATTERN_SIZE && sequence != std::numeric_limits<std::size_t>::max()))
			return false;

		_pattern_index = pattern;

		if (sequence != std::numeric_limits<std::size_t>::max()) {
			_patterns[_pattern_index].ChangeSequence(sequence);
			_changed_sequence = true;
		}

		return true;
	}
}