#include "Blynk/BlynkApp.hpp"

#include "Core/Sequencer.hpp"

#define BLYNK_PRINT stdout

#include <BlynkApiWiringPi.h>
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>

#include <cassert>

static BlynkTransportSocket G_blynkTransport;
BlynkSocket Blynk{ G_blynkTransport };
#include <BlynkWidgets.h>

Core::Sequencer* G_seq{ nullptr };
BlynkTimer G_synthlist_update_timer;

BLYNK_WRITE(V0)
{
	if (param.asInt() > 1) // Item 1 is "Off"
		G_seq->SelectInterface(param.asInt() - 2);
	else
		G_seq->CloseInterface();
}

BLYNK_WRITE(V1) // Mode switcher
{
	switch (param.asInt())
	{
		case 1: // Transpose
			G_seq->SetState(Core::Sequencer::State::Transpose);
			break;

		case 2: // Play
			G_seq->SetState(Core::Sequencer::State::Play);
			break;

		case 3: // Record
			G_seq->SetState(Core::Sequencer::State::Record);
			break;

		case 4: // Off
			G_seq->SetState(Core::Sequencer::State::Stop);
			break;

		default:
			// Hummm, something is wrong... TODO: Throw exception
			break;
	}
}

/*BLYNK_WRITE(V2) // Stepper Mode
{ G_seq->SetStepperMode(param.asInt() == 1); }*/

BLYNK_WRITE(V3) // Pattern selector
{ G_seq->SetPatternIndex(param.asInt() - 1); } // Blynk starts at 1, not 0

BLYNK_WRITE(V4) // Bank selector
{ G_seq->SetBankIndex(param.asInt() - 1); } // Blynk starts at 1, not 0

BLYNK_WRITE(V5) // Tap/Rest button
{ 
	if (param.asInt() == 1)
		G_seq->OnTap();
}

BLYNK_CONNECTED()
{ 
	Blynk.syncAll(); // Send all app values at connection
	
	Blynk.setProperty(V0, "labels", "Off", "No synth found");
} 

namespace BlynkApp
{
	void UpdateDeviceList()
	{
		BlynkParamAllocated items{ 128 };
		items.add("Off");
		const std::vector<std::string>& interfaces{ G_seq->GetInterfacesName() };
		if (interfaces.empty())
			items.add("No device found");
		else
		{
			for (const std::string& interface : interfaces)
				items.add(interface.c_str());
		}
		Blynk.setProperty(V0, "labels", items);
	}
	
	void Setup(Core::Sequencer* seq, const char* auth)
	{
		assert(seq != nullptr);

		G_seq = seq;
		Blynk.begin(auth);
		
		G_synthlist_update_timer.setInterval(2000L, UpdateDeviceList);
	}

	void Run()
	{
		Blynk.run();
		G_synthlist_update_timer.run();
	}
}
