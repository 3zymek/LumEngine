#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "core/logger.hpp"
using namespace lum;
int main() {

    Logger::Get().EnableLog(LogSeverity::DEBUG);
    
    core::render::RenderInitParams params;
    params.width = 1920;
    params.height = 1280;
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
    
    render::ShaderManager shaders;
    render::ShaderHandle basic_shader = shaders.CreateShader("basic.vert", "basic.frag");
    render::MeshManager mm;
    std::vector<core::render::Vertex> vertices = { 
        { {1, 1, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0} },
        { {-1, 1, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0} },
        { {0, -1, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0} }    
    };
    std::vector<core::render::Index> indices = { 0, 1, 2 };
    render::MeshHandle mesh_handle = mm.CreateStaticMesh(vertices, indices);

    while (renderer.WindowIsOpen()) {
        renderer.BeginFrame();

        bus.PollEvents();
        audio_system.Update();

        shaders.UseShader(basic_shader);
        mm.DrawMesh<render::StaticMesh>(mesh_handle);

        renderer.EndFrame();
    }
}
