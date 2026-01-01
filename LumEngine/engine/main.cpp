#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "core/logger.hpp"
#include "core/asset_service.hpp"
#include "render/core/mesh_manager.hpp"
using namespace lum;
int main() {

    Logger::Get().EnableLog(LogSeverity::ALL);
    
    core::render::RenderInitParams params;
    params.fullscreen = false;
    params.MSAA_samples = 4;
    params.title = "test";

    core::render::Renderer renderer;
    renderer.Init(params);

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

    render::MeshManager mm;
    std::vector<core::render::Vertex> vertices = { { {1, 1, 1}, {1,1,1}, {1,1,1} } };
    std::vector<core::render::Index> indices = { 0, 1, 2, 3 };
    render::MeshHandle handle = mm.CreateStaticMesh(vertices, indices);

    while (renderer.WindowIsOpen()) {
        renderer.BeginFrame();

        bus.PollEvents();
        audio_system.Update();

        renderer.EndFrame();
    }



    /*


    while (render.WindowIsOpen()) {
        render.BeginFrame();

        mm.DrawMesh<render::StaticMesh>(handle);

        render.EndFrame();
    }


    return 0;
    */
}
