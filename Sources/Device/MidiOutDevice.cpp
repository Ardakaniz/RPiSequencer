#include "Device/MidiOutDevice.hpp"

void MidiOutDevice::Open(unsigned int index) {
	if (index > _midi->getPortCount())
		throw std::runtime_error{ "[MidiOutDevice] Invalid index (" + std::to_string(index) + ")" };

	Close();

	_index = index;
	_midi->openPort(_index, "RPi Sequencer");
}

bool MidiOutDevice::IsOpen() const {
	return _midi->isPortOpen();
}

void MidiOutDevice::Close() {
	_midi->closePort();
}

std::string MidiOutDevice::GetDeviceType() const
{
	return "Midi Output Device";
}

std::string MidiOutDevice::GetDeviceName() const {
	if (!_midi->isPortOpen())
		throw std::runtime_error{ "[MidiOutDevice] Cannot get device name: Invalid device" };

	return _midi->getPortName(_index);
}

std::vector<std::string> MidiOutDevice::GetDeviceList() {
	std::vector<std::string> list;
	for (unsigned int i{ 0 }; i < _midi->getPortCount(); ++i)
		list.push_back(_midi->getPortName(i));

	return list;
}

void MidiOutDevice::PlayNote(const Core::Note& note) {
	unsigned char status_bit = 0b1001'0000 | (note.chan & 0x0F); // Note On
	unsigned char note_code  = 0b0111'1111 & note.note;
	unsigned char velocity   = 0b0111'1111 & note.velocity;

	std::vector<unsigned char> msg{ status_bit, note_code, velocity };
	_midi->sendMessage(&msg);
}

void MidiOutDevice::StopNote(const Core::Note& note) {
	unsigned char status_bit = 0b1000'0000 | (note.chan & 0x0F); // Note Off
	unsigned char note_code = 0b0111'1111 & note.note;
	unsigned char velocity = 0b0111'1111 & note.velocity;

	std::vector<unsigned char> msg{ status_bit, note_code, velocity };
	_midi->sendMessage(&msg);
}