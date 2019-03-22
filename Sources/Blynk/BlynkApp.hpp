#pragma once

#ifndef BLYNK_BLYNKAPP_HPP
#define BLYNK_BLYNKAPP_HPP

namespace Core
{ class Sequencer; }

namespace BlynkApp
{
	void Setup(Core::Sequencer* seq, const char* auth);
	void Run();
	void PulseTempoLED();
}

#endif // BLYNK_BLYNKAPP_HPP