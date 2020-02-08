#pragma once

#include "SequenceGenerator/SequenceGenerator.hpp"
#include "RtMidi/RtMidi.hpp"

#include <memory>

// namespace ext 

class MidiRecorder : public SequenceGenerator {
public:
	std::string GetType() const override;
	std::vector<std::pair<std::string, ParameterType>> GetParametersList() const override;
	void SetParameters(const std::vector<ParameterVariant>& parameters) override;
	
	void Start() override;
	void Stop() override;

private:
	bool Generate() override;

	std::unique_ptr<RtMidiIn> _midi{ std::make_unique<RtMidiIn>() };
	bool _is_device_open{ false };
	std::string _device_name{};
	std::size_t _index{ 0 };

	std::vector<std::pair<Core::Note, Core::TimePoint>> _notes_on, _notes_off;
};

// }
