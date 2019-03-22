#include "Blynk/BlynkApp.hpp"
#include "Core/Sequencer.hpp"

int main()
{
	Core::Sequencer seq{};
	seq.SetStepCallback([]() { BlynkApp::PulseTempoLED(); }); // When we step, pulse the tempo led

	constexpr const char* auth = "f511d2b876424efa9604783f4c032860";
	BlynkApp::Setup(&seq, auth);

	while(true)
	{
		BlynkApp::Run();
		seq.Run();
	}

	return 0;
}