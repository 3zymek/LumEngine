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
#include "imgui.h"
using namespace lum;
using namespace lum::rhi;
#define LUM_UNIFORM_BUFFER_STRUCT struct alignas(16)
class Camera {
public:

    Camera(Window* wind) : m_window(wind) { Init(wind); }
    
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
    Window* m_window = nullptr;

private:

    void RecalculateDirection() {

        glm::vec2 mouse_pos = input::GetMousePos();

        if (!mouse_locked) {
            lastX = mouse_pos.x;
            lastY = mouse_pos.y;
            return;
        }

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
        right = glm::normalize(glm::cross(front, glm::vec3(0,1,0)));
        up = glm::normalize(glm::cross(right, front));

    }

    void RecalculateMovement() {

        if (input::KeyPressed(input::Key::W)) {
            position += front * move_speed;
            //std::cout << "w";
        }
        if (input::KeyPressed(input::Key::S)) {
            position -= front * move_speed;
            //std::cout << "s";
        }
        if (input::KeyPressed(input::Key::A)) {
            position -= right * move_speed;
            //std::cout << "a";
        }
        if (input::KeyPressed(input::Key::D)) {
            position += right * move_speed;
            //std::cout << "d";
        }
        if (input::KeyPressedOnce(input::Key::LEFT_CONTROL)) {
            mouse_locked = !mouse_locked;
            if (mouse_locked) {
                glfwSetInputMode(static_cast<GLFWwindow*>(m_window->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else glfwSetInputMode(static_cast<GLFWwindow*>(m_window->GetNativeWindow()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
    float sensivity = 0.2f;
    bool mouse_locked = false;

    bool saveMousePos = false;
    bool mouseLocked = true;

};

LUM_UNIFORM_BUFFER_STRUCT CameraUBO {
    glm::mat4 view;
    glm::mat4 proj;
};
LUM_UNIFORM_BUFFER_STRUCT ModelUBO {
    glm::mat4 model;
    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rot;
};

int main() {

    Logger::Get().EnableLog(LogSeverity::ALL);
    Logger::Get().DisableLog(LogSeverity::DEBUG);

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
        {{ 10, 10, 0}, {0,0,1}, {2, 2}}, // top-right
        {{-10, 10, 0}, {0,0,1}, {0, 2}}, // top-left
        {{-10,-10, 0}, {0,1,0}, {0, 0}}, // bottom-left
        {{ 10,-10, 0}, {0,1,0}, {2, 0}}  // bottom-right
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
    ModelUBO modelu;
    rhi::BufferDescriptor uniformbuffer_descriptor2{
        .buffer_usage = BufferUsage::Dynamic,
        .size = sizeof(modelu),
        .map_flags = map_flags::Write,
        .data = nullptr
    };

    ///////////////////////////////////////////
    // Buffers
    ///////////////////////////////////////////
    auto vbo = device->CreateVertexBuffer(bdesc);
    auto ebo = device->CreateElementBuffer(indi);
    auto ubo = device->CreateUniformBuffer(uniformbuffer_descriptor);
    auto model_ubo = device->CreateUniformBuffer(uniformbuffer_descriptor2);
    device->SetUniformBufferBinding(model_ubo, LUM_UBO_MODEL_BINDING);
    device->SetUniformBufferBinding(ubo, LUM_UBO_CAMERA_BINDING);

    auto vao = device->CreateVertexLayout(vdesc, vbo);
    device->AttachElementBufferToLayout(ebo, vao);
    auto shader = device->CreateShader({ "basic.vert", "basic.frag" });
    
    TextureDescriptor tdescript;
    tdescript.filename = "test.jpg";
    tdescript.mag_filter = TextureMagFilter::Nearest;
    tdescript.min_filter = TextureMinFilter::Nearest;
    tdescript.anisotropy = 10;
    auto texture = device->CreateTexture2D(tdescript);

    glm::vec3 model_position    = { 0,0,0 };
    glm::vec3 model_scale       = { 1,1,1 };
    glm::vec3 model_rotation    = { 0,0,0 };

    while (window->IsOpen()) {

        device->BeginFrame();
        
        ImGui::Begin("Transform");
        ImGui::DragFloat3("position", glm::value_ptr(model_position), 0.1f, -1000, 1000);
        ImGui::DragFloat3("scale", glm::value_ptr(model_scale), 0.1f, -1000, 1000);
        ImGui::DragFloat3("rotation", glm::value_ptr(model_rotation), 0.1f, -1000, 1000);
        ImGui::End();

        glm::quat rot = glm::quat(glm::radians(model_rotation));
        glm::mat4 rotation = glm::mat4_cast(rot);
        modelu.model = glm::mat4(1.f);
        modelu.model = glm::translate(modelu.model, model_position);
        modelu.model = glm::scale(modelu.model, model_scale);
        modelu.model = modelu.model * rotation;

        c.Update();

        device->BindShader(shader);
        cubo.view = c.view;
        cubo.proj = c.projection;
        device->UpdateBuffer(model_ubo, &modelu, 0, 0);
        device->UpdateBuffer(ubo, &cubo, 0, 0);

        device->BindTexture(texture);
        device->DrawElements(vao, indices.size());

        device->EndFrame();
    }

}
