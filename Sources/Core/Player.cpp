#include "Core/Player.hpp"

namespace Core {
	Player::Player(const std::vector<std::shared_ptr<OutputDevice>>& devices) :
		_devices{ devices }
	{ }
}