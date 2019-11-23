#pragma once

#ifdef IMGUI_CONTROLLER

#include "Controller/Controller.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <vector>
#include <memory>

class ImGUIController : public Controller {
public:
	ImGUIController(const std::vector<std::shared_ptr<InputDevice>>& input_devices, const std::vector<std::shared_ptr<OutputDevice>>& output_devices);
	~ImGUIController();

	void Run() override;

private:
	sf::RenderWindow _window;
	sf::Clock _deltaClock;
	int _input_device_id{ 0 }, _input_port{ 0 };
	int _output_device_id{ 0 };
	int _seqmode;
	std::vector<bool> _output_devices_selected;
	std::vector<int> _output_ports;
};

#endif