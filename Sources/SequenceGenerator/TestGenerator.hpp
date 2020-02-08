#pragma once

#include "SequenceGenerator/SequenceGenerator.hpp"

class TestGenerator : public SequenceGenerator {
public:
	std::string GetType() const override;

private:
	bool Generate() override;
};