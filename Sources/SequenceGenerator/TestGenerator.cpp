#include "SequenceGenerator/TestGenerator.hpp"

std::string TestGenerator::GetType() const {
	return "Test Generator";
}

bool TestGenerator::Generate() {
	Core::Duration on_duration{ 500000000 }; // 1s
	Core::Duration total_duration{ 1000000000 }; // 0.5s
	_generated.push_back(Core::Note{ .note = 40, .velocity = 47, .on_duration = on_duration, .total_duration = total_duration });

	return true; // Generation is finished
}