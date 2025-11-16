#include <iostream>
#include "core/utils/path_service.hpp"
#include "lum_audio.hpp"
#include "lum_ecs.hpp"
#include "lum_events.hpp"
#include <thread>
#include <chrono>
int main() {
	
	PathService::SetRoot("assets");

	
	audio::AudioManager m;

	audio::AudioSystem sys(m);

	m.Init();
	
	m.LoadSound("test", "test.wav");

	Entity entity1;
	Entity entity2;

	auto* listener = entity1.AddComponent<AudioListenerComponent>();

	auto* emitter = entity2.AddComponent<AudioEmitterComponent>();

	m.CreateListener(entity1.GetID());

	auto ewrapper = m.CreateEmitter(emitter);

	ewrapper.Add("test").SetVolume("test", 1.1f).Play("test").SetPitch("test", 0.5);

	ev::EventBus::Engine().ProcessAll();
	sys.Update();

	system("pause");

	ewrapper.SetPaused("test", true);

	while (true) {
		ev::EventBus::Engine().ProcessAll();
		sys.Update();
	}

}