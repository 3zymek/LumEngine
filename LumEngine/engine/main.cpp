#include "ev_bus.hpp"
#include <iostream>
#include "audiomodule/audio_manager.hpp"
#include "utils/path_service.hpp"
int main() {
	PathService::SetRoot("assets");
	audio::AudioManager::Global().Init();
	audio::AudioManager::Global().LoadSound("test.wav", FMOD_2D);
	audio::AudioManager::Global().Play("test.wav");
	while(true) {}
}