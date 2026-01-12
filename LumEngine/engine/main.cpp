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


int main() {

    Logger::Get().EnableLog(LogSeverity::ALL);
    
    WindowDescriptor desc;
    auto* window = CreateWindow(desc);
    auto* device = CreateDevice(window);

    
    std::vector<VertexAttribute> attrib(3);
    auto& pos = attrib[0];
    pos.format = format::Float3;
    pos.relative_offset = offsetof(Vertex, position);
    pos.shader_location = 0;

    auto& color = attrib[1];
    color.format = format::Float3;
    color.relative_offset = offsetof(Vertex, color);
    color.shader_location = 1;

    auto& uv = attrib[2];
    uv.format = format::Float2;
    uv.relative_offset = offsetof(Vertex, uv);
    uv.shader_location = 2;

    VertexLayoutDescriptor vdesc;
    vdesc.stride = sizeof(Vertex);
    vdesc.attributes = attrib;

    std::vector<Vertex> verts = {
        {{ 1, 1, 0}, {0,0,0}, {1,1}}, // top-right
        {{-1, 1, 0}, {0,0,0}, {0,1}}, // top-left
        {{-1,-1, 0}, {0,0,0}, {0,0}}, // bottom-left
        {{ 1,-1, 0}, {0,0,0}, {1,0}}  // bottom-right
    };
    std::vector<unsigned int> indices = {
        0,1,2, 0,2,3
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

    TextureDescriptor tdescript;
    tdescript.filename = "test.jpg";
    tdescript.mag_filter = TextureMagFilter::Nearest;
    tdescript.min_filter = TextureMinFilter::Nearest;
    auto texture = device->CreateTexture2D(tdescript);

    while (window->IsOpen()) {
        device->BeginFrame();

        device->BindShader(shader);
        device->BindTexture(texture);
        device->DrawElements(vao, indices.size());

        device->EndFrame();
    }

}
