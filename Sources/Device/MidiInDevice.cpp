#include "Device/MidiInDevice.hpp"

void MidiInDevice::Open(unsigned int index) {
	if (index > _midi->getPortCount())
		throw std::runtime_error{ "[MidiInDevice] Invalid index (" + std::to_string(index) + ")" };

	Close();

	_index = index;
	_midi->openPort(_index, "RPi Sequencer");
}

void MidiInDevice::Close() {
	_midi->closePort();
}

std::string MidiInDevice::GetDeviceType() const
{ return "Midi Input Device"; }

std::string MidiInDevice::GetDeviceName() const {
	if (!_midi->isPortOpen())
		throw std::runtime_error{ "[MidiInDevice] Cannot get device name: Invalid device" };

	return _midi->getPortName(_index);
}

std::vector<std::string> MidiInDevice::GetDeviceList() {
	std::vector<std::string> list;
	for (unsigned int i{ 0 }; i < _midi->getPortCount(); ++i)
		list.push_back(_midi->getPortName(i));

	return list;
}

std::optional<std::pair<Core::Note, bool>> MidiInDevice::ReadNote() {
	std::vector<unsigned char> msg{};
	_midi->getMessage(&msg);

	if (msg.empty())
		return std::nullopt;

	Core::Note note;
	auto parse_msg = [&msg, &note]() -> std::optional<Core::Note> {
		if (msg.size() != 3)
			return std::nullopt; // Just ignore the message

		note.note = static_cast<std::uint8_t>(msg[1]);
		note.velocity = static_cast<std::uint8_t>(msg[2]);

		return note;
	};

	unsigned char status_bits = msg[0];

	note.chan = status_bits & 0x0F;
	
	bool note_pressed{ false };
	if ((status_bits & 0xF0) == 0b1001'0000)
		note_pressed = true;
	else if ((status_bits & 0xF0) != 0b1000'0000)
		return std::nullopt; // Just ignore the message

	auto filled_note = parse_msg();
	if (!filled_note) // God damn map function is still missing, Rust is better
		return std::nullopt;
	else
		return std::make_pair(*filled_note, note_pressed);
}