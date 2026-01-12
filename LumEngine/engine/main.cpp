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

void BenchmarkRHI(rhi::RHI_Device* device, size_t frames = 1000) {
    // ---------- Setup ----------
    std::vector<Vertex> verts = {
        {{1,1,0},{1,0,0}},
        {{-1,1,0},{0,1,0}},
        {{0,-1,0},{0,0,1}}
    };
    std::vector<unsigned int> indices = { 0,1,2 };

    rhi::BufferDescriptor vdesc{
        .buffer_usage = BufferUsage::Dynamic,
        .size = verts.size() * sizeof(Vertex),
        .data = verts.data()
    };
    rhi::BufferDescriptor idesc{
        .buffer_usage = BufferUsage::Dynamic,
        .size = indices.size() * sizeof(unsigned int),
        .data = indices.data()
    };

    auto vbo = device->CreateVertexBuffer(vdesc);
    auto ebo = device->CreateElementBuffer(idesc);

    std::vector<VertexAttribute> attrs = {
        {format::Float3, offsetof(Vertex, position), 0},
        {format::Float3, offsetof(Vertex, color), 1}
    };

    VertexLayoutDescriptor layout;
    layout.stride = sizeof(Vertex);
    layout.attributes = attrs;
    auto vao = device->CreateVertexLayout(layout, vbo);
    device->AttachElementBufferToLayout(ebo, vao);
    auto shader = device->CreateShader({ "basic.vert", "basic.frag" });

    // ---------- Benchmark loop ----------
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t frame = 0; frame < frames; ++frame) {
        device->BeginFrame();

        // zmieniamy dane
        for (auto& v : verts) {
            v.position[0] += 0.01f * ((frame % 2) ? 1 : -1);
            v.color[0] = float(frame % 255) / 255.0f;
        }

        // update buffer (Twój kontrakt: size=0 -> ca³y bufor)
        device->UpdateBuffer(vbo, verts.data(), 0, 0);

        device->BindShader(shader);
        device->DrawElements(vao, indices.size());

        device->EndFrame();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "[BenchmarkRHI] " << frames << " frames completed in " << diff << '\n';
}


int main() {

    Logger::Get().DisableLog(LogSeverity::ALL);
    
    WindowDescriptor desc;
    auto* window = CreateWindow(desc);
    auto* device = CreateDevice(window);

    BenchmarkRHI(device);

    /*
    std::vector<VertexAttribute> attrib(2);
    auto& pos = attrib[0];
    pos.format = format::Float3;
    pos.relative_offset = offsetof(Vertex, position);
    pos.shader_location = 0;

    auto& color = attrib[1];
    color.format = format::Float3;
    color.relative_offset = offsetof(Vertex, color);
    color.shader_location = 1;

    VertexLayoutDescriptor vdesc;
    vdesc.stride = sizeof(Vertex);
    vdesc.attributes = attrib;

    std::vector<Vertex> verts = {
        {{1,1,0}, {1,0,0}},
        {{-1,1,0}, {0,1,0}},
        {{0, -1, 0}, {0,0,1}}
    };
    std::vector<unsigned int> indices = {
        0,1,2
    };

    rhi::BufferDescriptor bdesc{
        .buffer_usage = BufferUsage::Dynamic,
        .size = verts.size() * sizeof(Vertex),
        .data = verts.data()
    };
    rhi::BufferDescriptor indi{
        .buffer_usage = BufferUsage::Dynamic,
        .size = bytesize(indices),
        .data = indices.data()
    };

    auto vbo = device->CreateVertexBuffer(bdesc);
    auto ebo = device->CreateElementBuffer(indi);

    auto vao = device->CreateVertexLayout(vdesc, vbo);
    device->AttachElementBufferToLayout(ebo, vao);
    auto shader = device->CreateShader({ "basic.vert", "basic.frag" });

    while (window->IsOpen()) {
        device->BeginFrame();

        device->BindShader(shader);
        device->DrawElements(vao, indices.size());

        device->EndFrame();
    }

   

    */

}
