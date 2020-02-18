#include "SequenceGenerator/TestGenerator.hpp"

std::string TestGenerator::GetType() const {
	return "Test Generator";
}

std::vector<std::pair<std::string, SequenceGenerator::ParameterType>> TestGenerator::GetParametersList() const {
	return { { "On Duration", ParameterType::UFLOAT },
	         { "Total Duration", ParameterType::UFLOAT },
	       };
}

bool TestGenerator::Generate() {
	const Core::Duration on_duration{ static_cast<unsigned int>(std::get<float>(_params[0]) * 1'000'000'000) };
	const Core::Duration total_duration{ static_cast<unsigned int>(std::get<float>(_params[1]) * 1'000'000'000) };
	_generated.push_back(Core::Note{ .note = 40, .velocity = 47, .on_duration = on_duration, .total_duration = total_duration });

	return true; // Generation is finished
}