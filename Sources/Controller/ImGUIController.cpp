#include "Controller/ImGUIController.hpp"

#ifdef IMGUI_CONTROLLER

#include "IMGUI/imgui.h"
#include "IMGUI/imgui-SFML.h"

#include <SFML/Window/Event.hpp>

#include "Device/Device.hpp"
#include "Core/Shared.hpp"

ImGUIController::ImGUIController(const std::vector<std::shared_ptr<SequenceGenerator>>& generators, const std::vector<std::shared_ptr<OutputDevice>>& output_devices) :
	Controller{ generators, output_devices },
	_seqmode{ Core::SeqMode_Stop }
{
	if (generators.empty())
		throw std::runtime_error{ "Empty generator list" };

	for (const auto& param : _generators[_generator_id /* = 0 */]->GetParametersList()) {
		switch (param.second) {
		case SequenceGenerator::ParameterType::UINT:
			_generators_params.emplace_back(0u);
			break;
		case SequenceGenerator::ParameterType::INT:
			_generators_params.emplace_back(0);
			break;
		case SequenceGenerator::ParameterType::FLOAT:
		case SequenceGenerator::ParameterType::UFLOAT:
			_generators_params.emplace_back(0.f);
			break;
		case SequenceGenerator::ParameterType::STR:
			_generators_params.emplace_back(std::string{});
			break;
		}
	}

	_window.create(sf::VideoMode(800, 600), "Sequencer Controller", sf::Style::Titlebar | sf::Style::Close);
	ImGui::SFML::Init(_window);
	_window.resetGLStates();

	_output_devices_selected.resize(_output_devices.size(), false);
	_output_ports.resize(_output_devices.size(), 0);


	/* ImGui Style */
	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
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
					if (_generator_id != i) {
						_generator_id = i;
						NewEvent({ .type = Event::UPDATE_GENERATOR, .data = _generators[i] });

						_generators_params.clear();
						_generators_params.resize(_generators[i]->GetParametersList().size());
					}
				}

				if (_generator_id == i) {
					const auto& param_list = _generators[i]->GetParametersList();

					bool updated{ false };
					for (std::size_t j = 0; j < param_list.size(); ++j) {
						switch (param_list[j].second) {
						case SequenceGenerator::ParameterType::UINT:
						{
							if (!std::holds_alternative<unsigned int>(_generators_params[j]))
								continue;

							int param = static_cast<int>(std::get<unsigned int>(_generators_params[j]));
							if (ImGui::InputInt((param_list[j].first + std::string{ "###param" } +std::to_string(j)).c_str(), &param)) {
								if (param < 0)
									param = 0;

								_generators_params[j] = static_cast<unsigned int>(param);
								updated = true;
							}
							break;
						}

						case SequenceGenerator::ParameterType::INT:
						{
							int* param = std::get_if<int>(&_generators_params[j]);
							if (param)
								updated |= ImGui::InputInt((param_list[j].first + std::string{ "###param" } +std::to_string(j)).c_str(), param);
							break;
						}

						case SequenceGenerator::ParameterType::FLOAT:
						{
							float* param = std::get_if<float>(&_generators_params[j]);
							if (param)
								updated |= ImGui::InputFloat((param_list[j].first + std::string{ "###param" } +std::to_string(j)).c_str(), param, 0.05f, 0.2f, "%.2f");
							break;
						}
						case SequenceGenerator::ParameterType::UFLOAT:
						{
							float* param = std::get_if<float>(&_generators_params[j]);
							if (param) {
								bool update = ImGui::InputFloat((param_list[j].first + std::string{ "###param" } +std::to_string(j)).c_str(), param, 0.05f, 0.2f, "%.2f");
								if (*param < 0)
									*param = 0;
								else
									updated |= update;
							}
							break;
						}
						case SequenceGenerator::ParameterType::STR:
							break;

						case SequenceGenerator::ParameterType::USERDATA:
							break;
						}
					}

					if (updated)
						_generators[i]->SetParameters(_generators_params);
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

			for (std::size_t i = 0; i < size; ++i) {
				ImGui::PushID(("sequencer_mutes_" + std::to_string(i)).c_str());
				bool isnt_muted{ !std::get<bool>(NewRequest({ .type = Request::SEQUENCE_STEP_MUTE, .param = i })) };
				if (ImGui::Checkbox("", &isnt_muted)) {
					if (!isnt_muted) // If it isn't not muted, we unmute it
						NewEvent({ .type = Event::UNMUTE_STEP, .data = i });
					else
						NewEvent({ .type = Event::MUTE_STEP, .data = i });
				}

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