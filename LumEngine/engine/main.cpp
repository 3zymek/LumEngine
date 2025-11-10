#include <iostream>
#include "audiomodule/audio_manager.hpp"
#include "utils/path_service.hpp"
#include "entitymodule/entity.hpp"
#include "entitymodule/components/c_audio_emitter.hpp"
int main() {
	PathService::SetRoot("assets");
	audio::AudioManager::Global().Init();
	audio::AudioManager::Global().LoadSound("test.wav", FMOD_2D);
	Entity e;
	auto comp = e.AddComponent<AudioEmitterComponent>();
	comp->emitterID = audio::AudioManager::Global().CreateEmitter();
	auto* emitter = audio::AudioManager::Global().GetEmitter(comp->emitterID);
	while(true) {}
}