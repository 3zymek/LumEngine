#include <iostream>
#include "utils/path_service.hpp"
#include "lum_audio.hpp"
#include "lum_ecs.hpp"
#include "lum_events.hpp"
int main() {
	PathService::SetRoot("assets");
	audio::AudioManager::Global().Init();
	audio::AudioManager::Global().LoadSound("test", "C:/Users/szymo/Desktop/LumEngine/LumEngine/assets/test.wav", FMOD_3D);
	Entity e;
	auto comp = e.AddComponent<AudioEmitterComponent>();
	auto audio = audio::AudioManager::Global().CreateEmitter();
	audio.AddClip("test");
	audio.PlayClip("test");
	while (true) { ev::EventBus::Global().ProcessAll(); }
}