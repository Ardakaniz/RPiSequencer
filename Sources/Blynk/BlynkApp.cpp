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
WidgetLED G_tempo_led(V3);
BlynkTimer G_timer;

BLYNK_WRITE(V0) // Mode switcher
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

BLYNK_WRITE(V1) // Tap/Rest button
{ 
	if (param.asInt() == 1)
		G_seq->OnTap();
}

BLYNK_WRITE(V2) // Rate slider
{ G_seq->SetRate(param.asFloat()); }

BLYNK_WRITE(V4) // Bank selector
{ G_seq->SetBankIndex(param.asInt() - 1); } // Blynk starts at 1, not 0

BLYNK_WRITE(V5) // Pattern selector
{ G_seq->SetPatternIndex(param.asInt() - 1); } // Blynk starts at 1, not 0

BLYNK_CONNECTED()
{ Blynk.syncAll(); } // Send all app values at connection

namespace BlynkApp
{
	void Setup(Core::Sequencer* seq, const char* auth)
	{
		assert(seq != nullptr);

		G_seq = seq;
		Blynk.begin(auth);
	}

	void Run()
	{ 
		Blynk.run();
		G_timer.run();
	}

	void TurnTempoLEDOff()
	{ G_tempo_led.off(); }

	void PulseTempoLED()
	{
		G_tempo_led.on();
		G_timer.setTimeout(300, TurnTempoLEDOff);
	}
}
