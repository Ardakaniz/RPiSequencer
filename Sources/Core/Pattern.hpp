#pragma once

#ifndef CORE_PATTERN_HPP
#define CORE_PATTERN_HPP

#include "Core/Note.hpp"

#include <vector>

namespace Core
{
	class Pattern
	{
	public:
		Pattern() = default;

		template<typename... Args> void AddNote(Args... args);
		void Clear();

		void Step();
		const Note& GetNote() const;

	private:
		std::vector<Note> _notes;
		unsigned int _step{ 0 };
	};
}

#include "Core/Pattern.inl"

#endif // CORE_PATTERN_HPP
