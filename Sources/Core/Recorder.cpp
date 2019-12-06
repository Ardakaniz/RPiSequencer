#include "Core/Recorder.hpp"

#include <algorithm>

namespace Core {
	void Recorder::SetDevice(std::shared_ptr<InputDevice> device) {
		if (_device)
			Stop();

		_device = device;
	}

	bool Recorder::Start(Pattern& pattern, unsigned int step_count) {
		if (!_device || !_device->IsOpen())
			return false;
		
		_pattern = pattern;
		_step_count = step_count;

		while (_device && _device->ReadNote()); // We clear the input buffer if existing

		if (_pattern->get().first.empty()) // If 
			_pattern->get().second = Clock::now();
		else
			_append_offset = Clock::now() - _pattern->get().first.back().release_instant;

		return true;
	}

	void Recorder::Stop() {
		if (!_pattern)
			return;

		for (std::size_t i : _pressed_notes_id) // If there are still pressed notes, we make them release now
			_pattern->get().first[i].release_instant = Clock::now();
		_pressed_notes_id.clear();

		_pattern = std::nullopt;
	}

	void Recorder::Run() {
		if (!IsRecording()) {
			while (_device && _device->ReadNote()); // We clear the input buffer if existing
			return;
		}

		std::optional<std::pair<Core::Note, bool /* is_pressed? */>> data = _device->ReadNote();
		if (!data)
			return;

		auto note = data->first;
		auto& pattern = _pattern->get().first;
		if (data->second) { // Note Pressed
			note.pressed_instant = Clock::now();

			if (_append_offset)
				note.pressed_instant -= *_append_offset; // If we append these notes, we skip the delay between last note and new recording start

			pattern.push_back(note);
			_pressed_notes_id.push_back(_pattern->get().first.size() - 1);
		}
		else { // Note Released
			auto corresponding_note_id = std::find_if(std::begin(_pressed_notes_id), std::end(_pressed_notes_id), [&note, &pattern, this](std::size_t id) { return note.note == pattern[id].note; });
			if (corresponding_note_id != std::end(_pressed_notes_id)) { // If we find a corresponding note (same note value)
				auto& pat = pattern[*corresponding_note_id];

				pat.release_instant = Clock::now(); // We set its release instant

				if (_append_offset)
					pat.release_instant -= *_append_offset;

				_pressed_notes_id.erase(corresponding_note_id); // And erase it from pressed notes
			}
		}
	}

	bool Recorder::IsRecording() const { 
		return (_device) && (_pattern) && (_step_count == 0 || (_pattern->get().first.size() + _pressed_notes_id.size()) < _step_count); // If the pattern isn't filled yet
	}
}