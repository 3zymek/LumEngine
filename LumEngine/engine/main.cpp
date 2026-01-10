#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "window_context/input_common.hpp"
#include "core/logger.hpp"
#include "editor.hpp"
#include "audio/components/c_audio_listener.hpp"
#include "window_context/window.hpp"
#include "rhi/core/rhi_device.hpp"
using namespace lum;
using namespace lum::rhi;
struct Bad {
    LumComponentTag;
    uint32_t id;
    std::string name;
};

int main() {

    Logger::Get().EnableLog(LogSeverity::ALL);
    
    WindowDescriptor desc;
    auto* window = CreateWindow(desc);
    auto* device = rhi::CreateDevice(window);

    std::vector<Vertex> verts = {
        {{1,1,0}, {1,0,0}},
        {{-1,1,0}, {0,1,0}},
        {{0, -1, 0}, {0,0,1}}
    };

    rhi::RHI_BufferDescriptor bdesc{
        .size = verts.size() * sizeof(Vertex),
        .flags = flags::Dynamic | flags::Vertex_Buffer | flags::Map_Write | flags::Map_Read
    };
    auto vbo = device->CreateBuffer(bdesc);

    device->UpdateBuffer(vbo, verts.data(), 0, 0, flags::Map_Read);

    while (window->IsOpen()) {
        device->BeginFrame();

        device->EndFrame();
    }

    /*
    core::EngineConfiguration cfg;

    cfg.render_config.fullscreen = false;
    cfg.render_config.height = 1280;
    cfg.render_config.width = 1920;
    cfg.render_config.title = "LumEngine Editor";

    core::Engine engine{ cfg };
    editor::Editor editor{ engine };
    Entity e = engine.GetECSManager().CreateEntity();
    e.AddComponent<ecs::components::TransformComponent>();
    e.AddComponent<ecs::components::MeshComponent>();

    auto& audio = engine.GetAudioManager();
    audio.LoadSound("01", "here_with_me.mp3");
    auto emitter = audio.CreateEmitter(e);
    emitter.Add("01");
    emitter.Play("01").SetVolume("01", 0.0);
    editor.Run();
    */

}
