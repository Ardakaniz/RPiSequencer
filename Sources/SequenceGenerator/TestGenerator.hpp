#pragma once

#include "SequenceGenerator/SequenceGenerator.hpp"

class TestGenerator : public SequenceGenerator {
public:
	std::string GetType() const override;

	std::vector<std::pair<std::string, ParameterType>> GetParametersList() const override;

private:
	bool Generate() override;
};