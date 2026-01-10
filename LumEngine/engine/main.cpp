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
#include "core/asset_service.hpp"
#include "rhi/rhi_common.hpp"
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
    auto* device = CreateDevice(window);

    std::vector<Vertex> verts = {
        {{1,1,0}, {1,0,0}},
        {{-1,1,0}, {0,1,0}},
        {{0, -1, 0}, {0,0,1}}
    };

    rhi::BufferDescriptor bdesc{
        .size = verts.size() * sizeof(Vertex),
        .buffer_type = BufferType::Vertex,
        .buffer_usage = BufferUsage::Dynamic,
        .map_flags = map_flags::Write
    };
    auto vbo = device->CreateBuffer(bdesc);

    void* ptr = device->MapBuffer(vbo, 0, 0, map_flags::Write);

    std::memcpy(ptr, verts.data(), verts.size() * sizeof(Vertex));

    device->UnmapBuffer(vbo);

    std::vector<VertexAttribute> attrib(2);
    auto& pos = attrib[0];
    pos.binding = 0;
    pos.format = format::Float3;
    pos.offset = 0;
    pos.relative_offset = offsetof(Vertex, position);
    pos.shader_location = 0;

    auto& color = attrib[1];
    color.binding = 0;
    color.format = format::Float3;
    color.offset = 0;
    color.relative_offset = offsetof(Vertex, color);
    color.shader_location = 1;

    VertexLayoutDescriptor vdesc;
    vdesc.stride = sizeof(Vertex);
    vdesc.attributes = attrib;

    auto vao = device->CreateVertexLayout(vdesc, vbo);

    auto shader = device->CreateShader({ "basic.vert", "basic.frag" });

    while (window->IsOpen()) {
        device->BeginFrame();

        device->BindShader(shader);
        device->Draw(vao, 3);

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
