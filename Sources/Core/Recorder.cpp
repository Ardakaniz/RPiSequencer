#include "Core/Recorder.hpp"

#include <algorithm>

namespace Core {
	Recorder::Recorder(std::shared_ptr<InputDevice> device) :
		_device{ device }
	{ }

	void Recorder::Start(Pattern& pattern, unsigned int step_count) {
		_pattern = pattern;
		_step_count = step_count;

		_pattern->get().second = Clock::now();
		_last_release_point = std::nullopt;
	}

	void Recorder::Stop() {
		if (!_pattern)
			return;

		const TimePoint last_release_instant{ _last_release_point.value_or(Clock::now()) };

		for (std::size_t i: _pressed_notes_id) // If there are still pressed notes, we make them release at the same time than the last released note or now if there arent any released note yet
			_pattern->get().first[i].release_instant = last_release_instant;
		_pressed_notes_id.clear();

		_pattern = std::nullopt;
	}

	void Recorder::Run() {
		if (!IsRecording())
			return;

		std::optional<std::pair<Core::Note, bool /* is_pressed? */>> data = _device->ReadNote();
		if (!data)
			return;

		auto note = data->first;
		auto& pattern = _pattern->get().first;
		if (data->second) { // Note Pressed
			note.pressed_instant = Clock::now();
			pattern.push_back(note);
			_pressed_notes_id.push_back(_pattern->get().first.size() - 1);
		}
		else { // Note Released
			auto corresponding_note_id = std::find_if(std::begin(_pressed_notes_id), std::end(_pressed_notes_id), [&note, &pattern, this](std::size_t id) { return note.note == pattern[id].note; });
			if (corresponding_note_id != std::end(_pressed_notes_id)) { // If we find a corresponding note (same note value)
				pattern[*corresponding_note_id].release_instant = Clock::now(); // We set its release instant
				_last_release_point = pattern[*corresponding_note_id].release_instant;

				_pressed_notes_id.erase(corresponding_note_id); // And erase it from pressed notes
			}
		}
	}

	bool Recorder::IsRecording() const { 
		return (_device) && (_pattern) && (_step_count == 0 || (_pattern->get().first.size() + _pressed_notes_id.size()) < _step_count); // If the pattern isn't filled yet
	}
}