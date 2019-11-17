#include "Core/Sequencer.hpp"
#include "Device/LogDevice.hpp"
#include "Device/MidiInDevice.hpp"
#include "Device/MidiOutDevice.hpp"
#include "Controller/ImGUIController.hpp"

int main() {
	std::vector<std::shared_ptr<InputDevice>> input_devices{
		std::make_shared<MidiInDevice>(),
	};

	std::vector<std::shared_ptr<OutputDevice>> output_devices{
		std::make_shared<LogDevice>(),
		std::make_shared<MidiOutDevice>(),
	};

	ImGUIController cont{ input_devices, output_devices };
	Core::Sequencer seq{ cont };

	while(!cont.IsStopped())
		seq.Run();
}