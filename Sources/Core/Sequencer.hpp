#pragma once

#include <array>

#include "Controller/Controller.hpp"
#include "Core/Player.hpp"
#include "Core/Pattern.hpp"

namespace Core {
	class Sequencer {
	public:
		Sequencer(Controller& controller);
		~Sequencer();

		void Run();

	private:
		bool ChangeSequence(std::size_t pattern, std::size_t sequence = std::numeric_limits<std::size_t>::max());

		Controller& _controller;
		std::shared_ptr<SequenceGenerator> _generator;
		Player _player;

		std::array<Pattern, BANK_SIZE> _patterns;
		std::size_t _pattern_index{ 0 };

		bool _changed_sequence{ true };
	};
}