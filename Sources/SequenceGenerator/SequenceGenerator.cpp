#include "SequenceGenerator/SequenceGenerator.hpp"

// namespace ext {

std::vector<std::pair<std::string, SequenceGenerator::ParameterType>> SequenceGenerator::GetParametersList() const {
	return {};
}

void SequenceGenerator::SetParameters(const std::vector<SequenceGenerator::ParameterVariant>& params) {
	_params = params;
}

void SequenceGenerator::Start() {
	_is_generating = true;
	_generated.clear();
}

void SequenceGenerator::Stop() {
	_is_generating = false;
}

bool SequenceGenerator::GenerationFinished() const {
	return !_is_generating;
}

std::vector<Core::Note> SequenceGenerator::Generated() {
	std::vector<Core::Note> generated = _generated;
	_generated.clear();
	return generated;
}

void SequenceGenerator::Run() {
	if (_is_generating)
		_is_generating = !Generate();
}

// }