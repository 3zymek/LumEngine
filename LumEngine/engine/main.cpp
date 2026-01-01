#include "core/core_pch.hpp"
#include "core/utils/path_service.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "core/logger.hpp"
#include "core/asset_service.hpp"

using namespace lum;
int main() {
    Logger::Get().EnableLog(LogSeverity::ALL);
    cstd::PathService::SetRoot("assets/audio");

    ev::EventBus bus;
    ecs::EntityManager ecs(bus);
    audio::AudioManager audio_manager(ecs);
    audio_manager.Init();
    audio_manager.LoadSound("audio01", "test.wav");
    audio::AudioSystem audio_system(audio_manager);

    Entity entity01 = ecs.CreateEntity();
    entity01.AddComponent<AudioEmitterComponent>();
    auto emitter = audio_manager.CreateEmitter(entity01);

    emitter.Add("audio01").SetVolume("audio01", 0.01f).Play("audio01");

    while (true) {
        bus.PollEvents();
        audio_system.Update();
    }



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
