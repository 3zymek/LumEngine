#include "ev_bus.hpp"
#include <iostream>
#include "audiomodule/audio_manager.hpp"
#include "utils/path_service.hpp"
<<<<<<< HEAD
int main() {
=======
#include "math/vec3f.hpp"
int main() {

>>>>>>> 2193e3f (fixed building, vectors3 with C core)
	PathService::SetRoot("assets");
	audio::AudioManager::Global().Init();
	audio::AudioManager::Global().LoadSound("test.wav", FMOD_2D);
	audio::AudioManager::Global().Play("test.wav");
	while(true) {}
}