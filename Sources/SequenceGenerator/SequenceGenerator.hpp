#pragma once

#include "Core/Note.hpp"

#include <string>
#include <variant>
#include <vector>

// namespace ext {

class SequenceGenerator {
public:
	enum class ParameterType {
		UINT,
		INT,
		FLOAT,
		STR,
		USERDATA
	};
	using ParameterVariant = std::variant<unsigned int, int, float, std::string, void*>;

	SequenceGenerator() = default;
	virtual ~SequenceGenerator() = default;

	virtual std::string GetType() const = 0;
	virtual std::vector<std::pair<std::string, ParameterType>> GetParametersList() const;
	virtual void SetParameters(const std::vector<ParameterVariant>& parameters);

	virtual void Start();
	virtual void Stop();

	void Run();
	bool GenerationFinished() const;
	std::vector<Core::Note> Generated();

protected:
	virtual bool Generate() = 0;

	std::vector<ParameterVariant> _params;
	std::vector<Core::Note> _generated;

	bool _is_generating{ false };
};

// }