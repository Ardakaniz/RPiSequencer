#include "Controller/ImGUIController.hpp"

#ifdef IMGUI_CONTROLLER

#include "IMGUI/imgui.h"
#include "IMGUI/imgui-SFML.h"

#include <SFML/Window/Event.hpp>

#include "Device/Device.hpp"
#include "Core/SequencerDef.hpp"

ImGUIController::ImGUIController(const std::vector<std::shared_ptr<InputDevice>>& input_devices, const std::vector<std::shared_ptr<OutputDevice>>& output_devices) :
	Controller{ input_devices, output_devices }
{
	_window.create(sf::VideoMode(800, 600), "Sequencer Controller", sf::Style::Titlebar | sf::Style::Close);
	ImGui::SFML::Init(_window);
	_window.resetGLStates();

	_output_devices_selected.resize(_output_devices.size(), false);
	_output_ports.resize(_output_devices.size(), 0);
}

ImGUIController::~ImGUIController() {
	ImGui::SFML::Shutdown();
}

void ImGUIController::Run() {
	if (_window.isOpen()) {
		sf::Event event;
		while (_window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				_window.close();
				_is_stopped = true;
			}
		}

		ImGui::SFML::Update(_window, _deltaClock.restart());

		if (ImGui::Begin("Devices")) {
			std::string input_names{ "None" };
			for (const auto& input : _input_devices)
				input_names += '\0' + input->GetDeviceType();
			input_names += '\0';

			if (ImGui::Combo("Input Device", &_input_device_id, input_names.c_str())) {
				if (_input_device_id > 0)
					Call({ .type = Event::INPUT_DEVICE, .data = _input_devices[_input_device_id - 1] });
			}

			if (_input_device_id > 0) {
				auto& input_device = _input_devices[_input_device_id - 1];
				std::string port_names{ "None" };
				for (const auto& port : input_device->GetDeviceList())
					port_names += '\0' + port;
				port_names += '\0';

				if (ImGui::Combo("Device Port", &_input_port, port_names.c_str())) {
					if (_input_port > 0)
						input_device->Open(_input_port - 1);
					else
						input_device->Close();
				}
			}

			ImGui::Separator();

			if (ImGui::ListBoxHeader("Output Devices")) {
				for (std::size_t i{ 0 }; i < _output_devices.size(); ++i) {
					if (ImGui::RadioButton(_output_devices[i]->GetDeviceType().c_str(), _output_devices_selected[i])) {
						_output_devices_selected[i] = !_output_devices_selected[i];
						Call({ .type = _output_devices_selected[i] ? Event::OUTPUT_DEVICE_ADD : Event::OUTPUT_DEVICE_REMOVE, .data = _output_devices[i] });
					}

					if (_output_devices_selected[i]) {
						auto& output_device = _output_devices[i];
						std::string port_names{ "None" };
						for (const auto& port : output_device->GetDeviceList())
							port_names += '\0' + port;
						port_names += '\0';
						std::string name = "Device Port " + std::to_string(i);
						if (ImGui::Combo(name.c_str(), &_output_ports[i], port_names.c_str())) {
							if (_output_ports[i] > 0)
								_output_devices[i]->Open(_output_ports[i] - 1);
							else
								_output_devices[i]->Close();
						}
					}
					else if (_output_ports[i] > 0) {
						_output_devices[i]->Close();
						_output_ports[i] = 0;
					}
				}
				ImGui::ListBoxFooter();
			}

		}
		ImGui::End();

		if (ImGui::Begin("Control")) {
			if (ImGui::Button("Play"))
				Call({ .type = Event::SEQUENCER_MODE, .data = static_cast<unsigned int>(Core::SeqMode_Play) });

			if (ImGui::Button("Record"))
				Call({ .type = Event::SEQUENCER_MODE, .data = static_cast<unsigned int>(Core::SeqMode_Record) });
		}
		ImGui::End();

		_window.clear();
		ImGui::SFML::Render(_window);
		_window.display();
	}
}

#endif