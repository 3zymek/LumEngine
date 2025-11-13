#include <iostream>
#include "utils/path_service.hpp"
#include "lum_audio.hpp"
#include "lum_ecs.hpp"
#include "lum_events.hpp"
int main() {
	
	PathService::SetRoot("assets");
	
	auto manager = audio::AudioManager::Global();
	manager.Init();
	manager.LoadSound("test", "test.wav", FMOD_3D);
	manager.LoadSound("test2", "test2.wav", FMOD_3D);
	audio::AudioSystem sys;
	Entity emitter;
	Entity listener;
	auto clis = listener.AddComponent<AudioListenerComponent>();
	auto cemi = emitter.AddComponent<AudioEmitterComponent>();
	auto wrapper = manager.CreateEmitter(cemi);
	wrapper.AddClip("test", 0.1f);
	wrapper.PlayClip("test", 0.1f);
	while (true) { sys.UpdateImplementation(); ev::EventBus::Engine().ProcessAll(); }
}