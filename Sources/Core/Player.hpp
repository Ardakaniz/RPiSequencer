#pragma once

#include <memory>
#include <vector>

#include "Device/Device.hpp"

namespace Core {
	class Player {
	public:
		Player(const std::vector<std::shared_ptr<OutputDevice>>& devices);

	private:
		std::vector<std::shared_ptr<OutputDevice>> _devices;
	};
}
