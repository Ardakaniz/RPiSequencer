#include "Controller/ImGUIController.hpp"

#ifdef IMGUI_CONTROLLER

#include "IMGUI/imgui.h"
#include "IMGUI/imgui-SFML.h"

#include <SFML/Window/Event.hpp>

#include "Device/Device.hpp"
#include "Core/Shared.hpp"
#include "SequenceGenerator/SequenceGenerator.hpp"

ImGUIController::ImGUIController(const std::vector<std::shared_ptr<SequenceGenerator>>& generators, const std::vector<std::shared_ptr<OutputDevice>>& output_devices) :
	Controller{ generators, output_devices },
	_seqmode{ Core::SeqMode_Stop }
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

		if (ImGui::Begin("Generators")) {
			for (std::size_t i{ 0 }; i < _generators.size(); ++i) {
				std::string id_str = "generator" + std::to_string(i);
				if (ImGui::RadioButton((_generators[i]->GetType() + "###" + id_str).c_str(), _generator_id == i)) {
					_generator_id = i;
					NewEvent({ .type = Event::UPDATE_GENERATOR, .data = _generators[i] });
				}
			}

			bool is_generating{ std::get<bool>(NewRequest({ .type = Request::IS_GENERATING })) };
			if (ImGui::Selectable("Generate", is_generating)) {
				if (is_generating)
					NewEvent({ .type = Event::STOP_GENERATION });
				else
					NewEvent({ .type = Event::GENERATE });
			}

		}
		ImGui::End();

		if (ImGui::Begin("Output Devices")) {
			for (std::size_t i{ 0 }; i < _output_devices.size(); ++i) {
				if (ImGui::RadioButton(_output_devices[i]->GetDeviceType().c_str(), _output_devices_selected[i])) {
					_output_devices_selected[i] = !_output_devices_selected[i];
					NewEvent({ .type = _output_devices_selected[i] ? Event::ADD_OUTPUT_DEVICE : Event::REMOVE_OUTPUT_DEVICE, .data = _output_devices[i] });
				}

				if (_output_devices_selected[i]) {
					auto& output_device = _output_devices[i];
					std::string port_names{ "None" };
					for (const auto& port : output_device->GetDeviceList())
						port_names += '\0' + port;
					port_names += '\0';

					if (_output_ports[i] >= output_device->GetDeviceList().size() + 1) {
						_output_ports[i] = 0;
						_output_devices[i]->Close();
					}

					std::string id = "Device Port " + std::to_string(i);
					if (ImGui::Combo(("DevicePort###" + id).c_str(), &_output_ports[i], port_names.c_str())) {
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
			ImGui::End();
		}

		if (ImGui::Begin("Control")) {
			int last_seqmode = _seqmode;
			if (ImGui::Selectable("Play", (_seqmode & 0xf) == Core::SeqMode_Play)) {
				_seqmode &= Core::SeqModeFlag_Mask; // Keep only flags
				_seqmode |= Core::SeqMode_Play;

				if (!NewEvent({ .type = Event::PLAY }))
					_seqmode = last_seqmode;
			}
			if (ImGui::Selectable("Transpose", (_seqmode & 0xf) == Core::SeqMode_Transpose)) {
				_seqmode &= Core::SeqModeFlag_Mask; // Keep only flags
				_seqmode |= Core::SeqMode_Transpose;

				if (!NewEvent({ .type = Event::TRANSPOSE }))
					_seqmode = last_seqmode;
			}
			if (ImGui::Selectable("Stop", (_seqmode & 0xf) == Core::SeqMode_Stop)) {
				_seqmode &= Core::SeqModeFlag_Mask; // Keep only flags
				_seqmode |= Core::SeqMode_Stop;

				if (!NewEvent({ .type = Event::STOP }))
					_seqmode = last_seqmode;
			}
		}
		ImGui::End();

		if (ImGui::Begin("Sequence")) {
			std::size_t size{ std::get<std::size_t>(NewRequest({ .type = Request::SEQUENCE_SIZE })) };

			for (std::size_t i = 0; i < size; ++i) {
				bool is_active{ std::get<std::size_t>(NewRequest({ .type = Request::SEQUENCE_STEP_INDEX })) == i };
				ImGui::PushID(("sequencer_step_" + std::to_string(i)).c_str());
					ImGui::RadioButton("", is_active);
				ImGui::PopID();
				ImGui::SameLine();
			}
			ImGui::NewLine();

			for (unsigned int i = 0; i < size; ++i) {
				ImGui::PushID(("sequencer_mutes_" + std::to_string(i)).c_str());
				bool b{ false };
				ImGui::Checkbox("", &b);
				ImGui::PopID();
				ImGui::SameLine();
			}
		}
		ImGui::End();

		_window.clear();
		ImGui::SFML::Render(_window);
		_window.display();
	}
}

#endif