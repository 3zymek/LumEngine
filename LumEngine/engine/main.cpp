#include "core/core_pch.hpp"
#include "core/utils/path_service.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"

#include "core/logger.hpp"

struct Event_Test {
    int val;
    LumEventTag;
};
struct TestEvent {
    int value;
    LumEventTag;
};
struct EventA {
	int value; LumEventTag;
};

struct EventB {
	float x; LumEventTag;
};

int main() {

    lum::Logger::Get().EnableLog(lum::LogSeverity::ALL);
    
    ev::EventBus bus;
    
    auto now = std::chrono::system_clock::now();
    bus.Subscribe<EventA>([](const EventA& ev) {std::cout << ev.value << '\n'; });
    auto id = bus.SubscribePermamently<EventB>([](const EventB& ev) {std::cout << "perm\n"; });
    auto id2 = bus.SubscribePermamently<EventB>([](const EventB& ev) {std::cout << "perm\n"; });
    auto id3 = bus.SubscribePermamently<EventB>([](const EventB& ev) {std::cout << "perm\n"; });

    bus.Emit(EventA{ 20 });
    bus.Emit(EventB{ 21 });
    bus.PollEvents();
    bus.UnsubscribePermament<EventB>(id);
    bus.UnsubscribePermament<EventB>(id2);
    bus.UnsubscribePermament<EventB>(id3);
    bus.Emit(EventB{ 21 });
    bus.PollEvents();


    auto end = std::chrono::system_clock::now();
    std::cout << std::chrono::duration<double>(end - now);


    /*
    ecs::EntityManager ecs;
    lum::AudioManager am(ecs);
    am.Init(512, FMOD_2D);
    lum::AudioSystem sys(am);

    Entity e = ecs.CreateEntity();
    e.AddComponent<AudioEmitterComponent>();
    auto emitter = am.CreateEmitter(e);
    am.LoadSound("atomic_land", "test.wav");
    emitter.Add("atomic_land");
    emitter.Play("atomic_land").SetVolume("atomic_land", 0.6f);

    sys.Update();

    while (true) {
        
        sys.Update();
    }

    render::RenderInitParams params;
    params.fullscreen = false;
    params.MSAA_samples = 4;
    params.title = "test";

    render::Renderer render;
    render.Init(params);

    render::MeshManager mm;
    std::vector<render::Vertex> vertices = { {{1, 1, 1}, {1,1,1}, {1,1,1}} };
    std::vector<render::Index> indices = { 0, 1, 2, 3 };
    render::MeshHandle handle = mm.CreateStaticMesh(vertices, indices);

    while (render.WindowIsOpen()) {
        render.BeginFrame();

        mm.DrawMesh<render::StaticMesh>(handle);

        render.EndFrame();
    }


    return 0;
    */
}
