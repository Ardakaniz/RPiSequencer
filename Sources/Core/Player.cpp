#include "Core/Player.hpp"

namespace Core {
	Player::Player(const std::vector<std::shared_ptr<OutputDevice>>& devices) :
		_devices{ devices }
	{ }

	void Player::Start(Pattern& pattern, bool loop) {
		_pattern = pattern;
		_loop = loop;
		Start();
	}

	void Player::Stop() {
		for (const Note& note : _played_note) // We stop all playing note
			StopNote(note);
		_played_note.clear();

		_pattern = std::nullopt;
	}

	void Player::Run() {
		if (!IsPlaying()) {
			if (_loop && _pattern)
				Start(); // If we must loop, we restart the player if possible when done
			else
				return;
		}

		auto now = Clock::now();
		if (_note_index < _pattern->get().first.size()) { // If there is still notes to play
			Note shifted_note = ShiftNoteTime(_pattern->get().first[_note_index]);
			if (now >= shifted_note.pressed_instant) {
				PlayNote(shifted_note);
				_played_note.push_back(shifted_note);
				++_note_index;
			}
		}

		// But whatever, we check always if there are notes to release
		std::vector<std::vector<Note>::const_iterator> to_release{}; // Stack note iterator to release
		for (auto it = std::begin(_played_note); it != std::end(_played_note); ++it) {
			if (now >= it->release_instant) {
				StopNote(*it);
				to_release.emplace_back(it);
			}
		}
		
		for (auto it : to_release) // To remove them from _played_note array
			_played_note.erase(it);
	}

	bool Player::IsPlaying() const {
		return (_pattern) && (_note_index < _pattern->get().first.size() || !_played_note.empty());
	}
	
	void Player::Start() {
		_start_point = Clock::now();
		_note_index = 0;
	}

	Note Player::ShiftNoteTime(const Note& note) {
		const Duration instant_offset = std::chrono::duration_cast<Duration>(_start_point - _pattern->get().second);
		
		Note new_note{ note };
		new_note.pressed_instant += instant_offset;
		new_note.release_instant += instant_offset;

		return new_note;
	}

	void Player::PlayNote(const Note& note) {
		for (auto device : _devices)
			device->PlayNote(note);
	}
	
	void Player::StopNote(const Note& note) {
		for (auto device : _devices)
			device->StopNote(note);
	}
}