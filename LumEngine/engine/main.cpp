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
#include "core/shaders_define.h"
#include "core/math/backend/gtx/string_cast.hpp"
using namespace lum;
using namespace lum::rhi;

class Camera {
public:

    Camera(Window* wind) { Init(wind); }
    
    void Update() {

        RecalculateDirection();
        RecalculateMovement();
        RecalculateMVP();

    }

    glm::mat4 view = glm::mat4(1.f);
    glm::mat4 projection = glm::mat4(1.f);

    glm::vec3 position = { 0,0,0 };

    glm::vec3 front = { 0,0,-1 };
    glm::vec3 up    = { 0,1,0 };
    glm::vec3 right = { 1,0,0 };


private:

    void RecalculateDirection() {

        glm::vec2 mouse_pos = input::GetMousePos();

        float xpos = mouse_pos.x - lastX;
        float ypos = lastY - mouse_pos.y;

        lastX = mouse_pos.x;
        lastY = mouse_pos.y;

        xpos *= sensivity;
        ypos *= sensivity;

        yaw += xpos;
        pitch += ypos;

        if (pitch >= 89.f) pitch = 89.f;
        if (pitch <= -89.f) pitch = -89.f;

        glm::vec3 new_view = { 0,0,0 };
        new_view.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_view.y = sin(glm::radians(pitch));
        new_view.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(new_view);
        right = glm::normalize(glm::cross(front, up));
        //up = glm::normalize(glm::cross(front, right));

        //std::cout << pitch << '\n';

    }

    void RecalculateMovement() {

        if (input::KeyPressed(input::Key::W)) {
            position += front * move_speed;
            std::cout << "w";
        }
        if (input::KeyPressed(input::Key::S)) {
            position -= front * move_speed;
            std::cout << "s";
        }
        if (input::KeyPressed(input::Key::A)) {
            position -= right * move_speed;
            std::cout << "a";
        }
        if (input::KeyPressed(input::Key::D)) {
            position += right * move_speed;
            std::cout << "d";
        }
        if (input::KeyPressedOnce(input::Key::LEFT_CONTROL)) {
            mouse_locked = !mouse_locked;
        }

    }

    void RecalculateMVP() {

        view = glm::lookAt(position, front + position, up);
        projection = glm::perspective(glm::radians(fov), aspect_ratio, min_plane, max_plane);

    }

    void Init(Window* wind) {
        aspect_ratio = (double)wind->GetWidth() / (double)wind->GetHeight();
        glm::vec2 mouse_pos = input::GetMousePos();
        lastX = mouse_pos.x;
        lastY = mouse_pos.y;
    }
    
    float move_speed = 0.1f;
    float aspect_ratio = 0;
    float lastX = 0;
    float lastY = 0;
    float yaw = 0, pitch = 0;
    float fov = 45.f;
    float min_plane = 0.1f;
    float max_plane = 100000.f;
    float sensivity = 0.3f;
    bool mouse_locked = false;

    bool saveMousePos = false;
    bool mouseLocked = true;

};
struct alignas(16) CameraUBO {
    glm::mat4 view;
    glm::mat4 proj;
};

int main() {

    Logger::Get().EnableLog(LogSeverity::ALL);

    ///////////////////////////////////////////
    // Init
    ///////////////////////////////////////////
    WindowDescriptor desc;
    desc.MSAA_samples = 4;
    desc.height = 1440;
    desc.width = 5440;
    auto* window = CreateWindow(desc);
    auto* device = CreateDevice(window);
    input::SetActiveWindow(static_cast<GLFWwindow*>(window->GetNativeWindow()));
    Camera c{ window };

    ///////////////////////////////////////////
    // Attributes
    ///////////////////////////////////////////
    std::vector<VertexAttribute> attrib(3);
    auto& pos = attrib[0];
    pos.format = format::Float3;
    pos.relative_offset = offsetof(Vertex, position);
    pos.shader_location = LUM_LAYOUT_POSITION;

    auto& color = attrib[1];
    color.format = format::Float3;
    color.relative_offset = offsetof(Vertex, color);
    color.shader_location = LUM_LAYOUT_COLOR;

    auto& uv = attrib[2];
    uv.format = format::Float2;
    uv.relative_offset = offsetof(Vertex, uv);
    uv.shader_location = LUM_LAYOUT_UV;

    VertexLayoutDescriptor vdesc;
    vdesc.stride = sizeof(Vertex);
    vdesc.attributes = attrib;

    std::vector<Vertex> verts = {
        {{ 1, 1, 0}, {0,0,1}, {1, 1}}, // top-right
        {{-1, 1, 0}, {0,0,1}, {0, 1}}, // top-left
        {{-1,-1, 0}, {0,1,0}, {0, 0}}, // bottom-left
        {{ 1,-1, 0}, {0,1,0}, {1, 0}}  // bottom-right
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
    CameraUBO cubo;
    cubo.view = c.view;
    cubo.proj = c.projection;
    rhi::BufferDescriptor uniformbuffer_descriptor{
        .buffer_usage = BufferUsage::Dynamic,
        .size = sizeof(cubo),
        .map_flags = map_flags::Write,
        .data = &cubo
    };

    ///////////////////////////////////////////
    // Buffers
    ///////////////////////////////////////////
    auto vbo = device->CreateVertexBuffer(bdesc);
    auto ebo = device->CreateElementBuffer(indi);
    auto ubo = device->CreateUniformBuffer(uniformbuffer_descriptor);
    device->SetUniformBufferBinding(ubo, LUM_UBO_CAMERA_BINDING);

    auto vao = device->CreateVertexLayout(vdesc, vbo);
    device->AttachElementBufferToLayout(ebo, vao);
    auto shader = device->CreateShader({ "basic.vert", "basic.frag" });
    
    TextureDescriptor tdescript;
    tdescript.filename = "drzewo.jpg";
    tdescript.mag_filter = TextureMagFilter::Nearest;
    tdescript.min_filter = TextureMinFilter::Nearest;
    auto texture = device->CreateTexture2D(tdescript);
    glm::vec3 position = { 0,0,0 };
    glm::vec3 scale = { 1,1,1 };
    while (window->IsOpen()) {

        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);

        device->BeginFrame();

        c.Update();

        device->BindShader(shader);
        cubo.view = c.view;
        cubo.proj = c.projection;
        device->UpdateBuffer(ubo, &cubo, 0, 0);
        
        device->SetMat4(shader, LUM_MODEL_MAT4_STR, model);

        device->BindTexture(texture);
        device->DrawElements(vao, indices.size());

        device->EndFrame();
    }

}
