#include "Core/Player.hpp"

#include <algorithm>

namespace Core {
	void Player::AddDevice(std::shared_ptr<OutputDevice> device) {
		auto it = std::find(std::begin(_devices), std::end(_devices), device);
		if (it == std::end(_devices))
			_devices.push_back(device);
	}

	void Player::RemoveDevice(std::shared_ptr<OutputDevice> device) {
		auto it = std::find(std::begin(_devices), std::end(_devices), device);
		if (it != std::end(_devices))
			_devices.erase(it);
	}

	void Player::Start(bool loop) {
		_loop = loop;
		_stopped = false;
	}

	void Player::Start(Sequence& seq, bool loop) {
		if (seq.GetSize() == 0)
			return;

		_sequence = seq;
		_loop = loop;
		Restart();
	}

	void Player::Restart() {
		_stopped = false;

		_note_index = 0;
		_change_note_instant = TimePoint::min();

		PlayCurrentNote(Clock::now()); // Play the first note
	}

	void Player::EnableStepperMode(bool enable) {
		_stepper_mode = enable;
	}

	void Player::Stop() {
		for (const auto& note : _playing_note) // We stop all playing note
			StopNote(note.first);
		_playing_note.clear();

		_stopped = true;
	}

	void Player::Run() {
		if (!IsPlaying()) {
			if (_loop && _sequence && !_stopped)
				Restart(); // If we must loop, we restart the player if possible when done
			else
				return;
		}

		auto now = Clock::now();

		if (now >= _change_note_instant) {
			++_note_index;

			if (_note_index < _sequence->get().GetSize())
				PlayCurrentNote(now);
		}

		// We check if there are notes to release
		auto remove_it = std::remove_if(std::begin(_playing_note), std::end(_playing_note), [&now, this](const auto& note) {
			if (now >= note.first.on_duration + note.second) {
				StopNote(note.first);
				return true;
			}

			return false;
		});
		_playing_note.erase(remove_it, std::end(_playing_note));
	}

	bool Player::IsPlaying() const {
		return _sequence && !_stopped && (_note_index < _sequence->get().GetSize() || !_playing_note.empty());
	}

	std::size_t Player::GetNoteIndexPlaying() const {
		return _note_index;
	}

	void Player::PlayCurrentNote(TimePoint now) {
		const auto& note = _sequence->get().GetNote(_note_index);

		_change_note_instant = now + note.first.total_duration;
		if (note.second) { // If the note is not muted, we play it
			_playing_note.emplace_back(note.first, now);

			for (auto device : _devices)
				device->PlayNote(note.first);
		}
	}
	
	void Player::StopNote(const Note& note) {
		for (auto device : _devices)
			device->StopNote(note);
	}
}