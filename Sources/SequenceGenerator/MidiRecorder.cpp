#include "SequenceGenerator/MidiRecorder.hpp"

#include <algorithm>

std::string MidiRecorder::GetType() const {
	std::string name = "Recorder";
	if (_is_device_open)
		name += ": " + _device_name;
	return name;
}

std::vector<std::pair<std::string, SequenceGenerator::ParameterType>> MidiRecorder::GetParametersList() const {
	return { { "Port Index", ParameterType::UINT } };
};

void MidiRecorder::SetParameters(const std::vector<ParameterVariant>& parameters) {
	SequenceGenerator::SetParameters(parameters);

	unsigned int index = std::get<unsigned int>(_params[0]);
	if (index > _midi->getPortCount())
		throw std::runtime_error{ "[MidiOutDevice] Invalid index (" + std::to_string(index) + ")" };

	_index = index;
}

void MidiRecorder::Start() {
	SequenceGenerator::Start();

	_midi->closePort();
	_midi->openPort(_index, "RPi Sequencer");
	_device_name = _midi->getPortName(_index);

	_is_device_open = true;
}

void MidiRecorder::Stop() {
	SequenceGenerator::Stop();

	auto now = Core::Clock::now();
	for (auto& note : _notes_on) {
		note.first.total_duration = std::chrono::duration_cast<Core::Duration>(now - note.second);
		_generated.push_back(note.first);
	}
	for (auto& note : _notes_off) {
		note.first.total_duration = std::chrono::duration_cast<Core::Duration>(now - note.second);
		_generated.push_back(note.first);
	}
	_notes_on.clear();
	_notes_off.clear();

	_midi->closePort();

	_is_device_open = false;
}

bool MidiRecorder::Generate() {
	auto now = Core::Clock::now();

	std::vector<unsigned char> msg{};
	_midi->getMessage(&msg);

	if (msg.size() != 3)
		return false; // Just ignore message w/o stopping recording

	unsigned char status_bits = msg[0];

	bool note_pressed = false;
	if ((status_bits & 0xF0) == 0b1001'0000)
		note_pressed = true;
	else if ((status_bits & 0xF0) != 0b1000'0000)
		return false; // Just ignore message w/o stopping recording

	std::uint8_t note_val = static_cast<std::uint8_t>(msg[1]);
	if (note_pressed) {
		// If a new note is pressed, we set total_duration to either the notes on or the notes off
		for (auto& note : _notes_on)
			note.first.total_duration = std::chrono::duration_cast<Core::Duration>(now - note.second);
		for (auto& note : _notes_off) {
			note.first.total_duration = std::chrono::duration_cast<Core::Duration>(now - note.second);
			_generated.push_back(note.first);
		}
		_notes_off.clear();

		Core::Note note;
		note.note = note_val;
		note.velocity = static_cast<std::uint8_t>(msg[2]);
		note.chan = status_bits & 0x0F;

		_notes_on.emplace_back(note, now); // now = time point when note pressed
	}
	else { // Note released
		auto note_it = std::remove_if(std::begin(_notes_on), std::end(_notes_on), [note_val](const auto& note) { return note.first.note == note_val; });
		if (note_it != std::end(_notes_on)) {
			note_it->first.on_duration = std::chrono::duration_cast<Core::Duration>(now - note_it->second); // now = time point when note released
			_notes_off.push_back(*note_it);
		}
		_notes_on.erase(note_it, std::end(_notes_on));
	}

	return false; // Recording is never finished
}