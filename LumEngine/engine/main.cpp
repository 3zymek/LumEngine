#include <iostream>
#include "core/utils/path_service.hpp"
#include "lum_audio.hpp"
#include "lum_ecs.hpp"
#include "lum_events.hpp"
#include "glad/glad.h"
#include "glfw3.h"
#include "imgui.h"
int main() {

	IMGUI_CHECKVERSION();

	cstd::PathService::SetRoot("assets");

	ecs::EntityManager ecs;
	
	audio::AudioManager m(ecs);

	audio::AudioSystem sys(m);

	m.Init(100, FMOD_INIT_NORMAL);
	
	m.LoadSound("test", "test.wav", FMOD_2D);
	m.LoadSound("test2", "test2.wavaw");

	Entity entity1 = ecs.CreateEntity();
	Entity entity2 = ecs.CreateEntity();
	
	std::cout << ecs.Has<TransformComponent>(entity1.GetID());

	auto* listener = entity1.AddComponent<AudioListenerComponent>();
	auto* emitter = entity2.AddComponent<AudioEmitterComponent>();

	m.CreateListener(entity1.GetID());

	auto lwrapper = m.CreateListener(entity1.GetID());
	auto ewrapper = m.CreateEmitter(emitter);

	ewrapper.Add("test")
		.SetVolume("test", 1.f)
		.Play("test")
		.SetPitch("test", 0.5);

	ewrapper.Add("test2")
		.SetVolume("test2", 1.5f);

	ev::EventBus::Engine().ProcessAll();
	sys.Update();

	auto* transform = entity1.GetComponent<TransformComponent>();

	system("pause");

	ewrapper.SetPaused("test", true);

	ev::EventBus::Engine().ProcessAll();
	sys.Update();

	system("pause");

	ewrapper.Stop("test2");
	ewrapper.SetPaused("test", false);

	ev::EventBus::Engine().ProcessAll();
	sys.Update();

	system("pause");

	ewrapper.SetVolume("test", 0.f);


	while (true) {
		ev::EventBus::Engine().ProcessAll();
		sys.Update();
	}

}