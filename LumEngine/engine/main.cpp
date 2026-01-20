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
#include "core/fixed_string.hpp"
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
    //Logger::Get().DisableLog(LogSeverity::DEBUG);

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
    pos.format = DataFormat::Float3;
    pos.relative_offset = offsetof(Vertex, position);
    pos.shader_location = LUM_LAYOUT_POSITION;

    auto& color = attrib[1];
    color.format = DataFormat::Float3;
    color.relative_offset = offsetof(Vertex, color);
    color.shader_location = LUM_LAYOUT_COLOR;

    auto& uv = attrib[2];
    uv.format = DataFormat::Float2;
    uv.relative_offset = offsetof(Vertex, uv);
    uv.shader_location = LUM_LAYOUT_UV;

    VertexLayoutDescriptor vao_description;
    vao_description.stride = sizeof(Vertex);
    vao_description.attributes = attrib;

    std::vector<Vertex> verts = {
        {{ 10, 10, 0}, {0,0,1}, {2, 2}}, // top-right
        {{-10, 10, 0}, {0,0,1}, {0, 2}}, // top-left
        {{-10,-10, 0}, {0,1,0}, {0, 0}}, // bottom-left
        {{ 10,-10, 0}, {0,1,0}, {2, 0}}  // bottom-right
    };
    std::vector<unsigned int> indices = {
        0,1,2, 0,2,3
    };

    rhi::BufferDescriptor vbo_description{
        .buffer_usage = BufferUsage::Dynamic,
        .size = verts.size() * sizeof(Vertex),
        .data = verts.data()
    };
    rhi::BufferDescriptor ebo_description{
        .buffer_usage = BufferUsage::Dynamic,
        .size = bytesize(indices),
        .data = indices.data()
    };
    CameraUBO cubo;
    cubo.view = c.view;
    cubo.proj = c.projection;
    rhi::BufferDescriptor ubo_description{
        .buffer_usage = BufferUsage::Dynamic,
        .size = sizeof(cubo),
        .map_flags = map_flags::Write,
        .data = &cubo
    };
    ModelUBO UBO_model;
    rhi::BufferDescriptor ubo_description_2{
        .buffer_usage = BufferUsage::Dynamic,
        .size = sizeof(UBO_model),
        .map_flags = map_flags::Write,
        .data = nullptr
    };
    SamplerDescriptor samplerdesc{
        .mag_filter = SamplerMagFilter::Nearest,
        .min_filter = SamplerMinFilter::Linear_mipmap_linear,
        .anisotropy = 16,
    };

    ///////////////////////////////////////////
    // Buffers
    ///////////////////////////////////////////
    auto vbo = device->CreateVertexBuffer(vbo_description);
    auto ebo = device->CreateElementBuffer(ebo_description);
    auto ubo = device->CreateUniformBuffer(ubo_description);
    auto sampler = device->CreateSampler(samplerdesc);
    auto model_ubo = device->CreateUniformBuffer(ubo_description_2);
    device->SetUniformBufferBinding(model_ubo, LUM_UBO_MODEL_BINDING);
    device->SetUniformBufferBinding(ubo, LUM_UBO_CAMERA_BINDING);
    device->SetSamplerBinding(sampler, LUM_TEXTURE_BINDING_01);

    auto vao = device->CreateVertexLayout(vao_description, vbo);
    device->AttachElementBufferToLayout(ebo, vao);
    auto shader = device->CreateShader({ "basic.vert", "basic.frag" });
    
    TextureDescriptor texture_description;
    texture_description.filename = "test.jpg";
    texture_description.mipmaping = true;
    auto texture = device->CreateTexture2D(texture_description);
    device->SetTextureBinding(texture, LUM_TEXTURE_BINDING_01);

    glm::vec3 model_position    = { 0,0,0 };
    glm::vec3 model_scale       = { 1,1,1 };
    glm::vec3 model_rotation    = { 0,0,0 };

    while (window->IsOpen()) {

        glm::quat rot = glm::quat(glm::radians(model_rotation));
        glm::mat4 rotation = glm::mat4_cast(rot);
        UBO_model.model = glm::mat4(1.f);
        UBO_model.model = glm::translate(UBO_model.model, model_position);
        UBO_model.model = UBO_model.model * rotation;
        UBO_model.model = glm::scale(UBO_model.model, model_scale);

        device->BeginFrame();
        
        ImGui::Begin("Transform");
        ImGui::DragFloat3("position", glm::value_ptr(model_position), 0.1f, -1000, 1000);
        ImGui::DragFloat3("scale", glm::value_ptr(model_scale), 0.1f, -1000, 1000);
        ImGui::DragFloat3("rotation", glm::value_ptr(model_rotation), 0.1f, -1000, 1000);
        ImGui::End();

        c.Update();

        device->BindShader(shader);
        cubo.view = c.view;
        cubo.proj = c.projection;
        device->UpdateBuffer(model_ubo, &UBO_model, 0, 0);
        device->UpdateBuffer(ubo, &cubo, 0, 0);

        device->BindSampler(sampler);
        device->BindTexture(texture);
        device->DrawElements(vao, indices.size());

        device->EndFrame();
    }

}
