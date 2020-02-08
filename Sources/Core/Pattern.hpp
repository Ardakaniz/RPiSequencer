#pragma once

#include "Core/Shared.hpp"
#include "Core/Sequence.hpp"

#include <array>

namespace Core {
	class Pattern {
	public:
		Pattern() = default;

		void ChangeSequence(std::size_t id);
		bool SetSequenceSize(std::size_t new_size);

		Sequence& GetSequence();
		const Sequence& GetSequence() const;

	private:
		std::array<Sequence, PATTERN_SIZE> _sequences{ // Ugly thing to init the whole array
			std::apply([this](auto... xs) {
				return std::array<Sequence, PATTERN_SIZE>{((void)xs, *this)...};
			}, std::array<char, PATTERN_SIZE>{})
		};

		std::size_t _current_sequence{ 0 };
	};
}