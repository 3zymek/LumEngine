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

int main() {
    
    lum::Logger::Get().EnableLog(lum::LogSeverity::INFO);

    cstd::PathService::SetRoot("assets/audio");

    ev::EventBus a;



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
