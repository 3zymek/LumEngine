#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "input/input_common.hpp"
#include "core/logger.hpp"
#include "editor.hpp"
#include "render/components/c_mesh.hpp"
#include "audio/components/c_audio_listener.hpp"
using namespace lum;
struct Bad {
    LumComponentTag;
    uint32_t id;
    std::string name;
};

int main() {

    Logger::Get().DisableLog(LogSeverity::DEBUG);
    
    core::EngineConfiguration cfg;

    cfg.render_config.fullscreen = true;
    cfg.render_config.height = 1280;
    cfg.render_config.width = 1920;
    cfg.render_config.title = "LumEngine Editor";

    core::Engine engine{ cfg };
    editor::Editor editor{ engine };
    Entity e = engine.GetECSManager().CreateEntity();
    e.AddComponent<ecs::components::TransformComponent>();
    e.AddComponent<ecs::components::MeshComponent>();

    auto& audio = engine.GetAudioManager();
    audio.LoadSound("01", "test2.wav");
    auto emitter = audio.CreateEmitter(e);
    emitter.Add("01");
    emitter.Play("01").SetVolume("01", 0.5);
    editor.Run();



    /*

    render::RenderConfig params;
    params.width = 1920;
    params.height = 1280;
    params.fullscreen = false;
    params.MSAA_samples = 4;
    params.title = "test";

    render::Renderer renderer;
    renderer.Init(params);
    ev::EventBus bus;
    ecs::EntityManager ecs(bus);
    audio::AudioManager audio_manager(ecs, bus);
    audio_manager.Init();
    audio_manager.LoadSound("audio01", "test.wav");
    audio::AudioSystem audio_system(audio_manager);

    input::SetActiveWindow(renderer.GetWindow());

    Entity entity01 = ecs.CreateEntity();
    entity01.AddComponent<ecs::components::AudioEmitterComponent>();
    entity01.AddComponent<ecs::components::TransformComponent>();
    auto emitter = audio_manager.CreateEmitter(entity01);

    emitter.Add("audio01").SetVolume("audio01", 0.5f).Play("audio01");
    
    render::ShaderManager shaders;
    render::ShaderHandle basic_shader = shaders.CreateShader("basic.vert", "basic.frag");
    render::MeshManager mm;
    std::vector<render::Vertex> vertices = { 
        { {1, 1, 0}, {1, 0, 0}, {0, 0, 0}, {0, 0} },
        { {-1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {0, 0} },
        { {0, -1, 0}, {0, 0, 1}, {0, 0, 0}, {0, 0} }    
    };
    std::vector<render::Index> indices = { 0, 1, 2 };
    render::MeshHandle mesh_handle = mm.CreateStaticMesh(vertices, indices);
    while (renderer.WindowIsOpen()) {
        renderer.BeginFrame();

        bus.PollEvents();
        audio_system.Update();

        if (input::KeyPressedOnce(input::Key::SPACE)) {
            emitter.SetPaused("audio01", !emitter.GetPaused("audio01"));
        }
        if (input::KeyPressedOnce(input::Key::UP)) {
            emitter.SetVolume("audio01", emitter.GetVolume("audio01") + 0.1);
        }
        if (input::KeyPressedOnce(input::Key::DOWN)) {
            emitter.SetVolume("audio01", emitter.GetVolume("audio01") - 0.1);
        }

        shaders.UseShader(basic_shader);
        mm.DrawMesh<render::StaticMesh>(mesh_handle);

        renderer.EndFrame();
    }*/
}
