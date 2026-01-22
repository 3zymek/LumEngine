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

std::vector<Vertex> verts = {
    {{ 0.5, 0.5, 0}, {0,0,1}}, // top-right
    {{-0.5, 0.5, 0}, {0,0,1}}, // top-left
    {{-0.5,-0.5, 0}, {0,1,0}}, // bottom-left
    {{ 0.5,-0.5, 0}, {0,1,0}}  // bottom-right
};
std::vector<unsigned int> indices = {
    0,1,2, 0,2,3
};
float quad[] = {
    // pos      // uv
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,

    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f
};

int main() {

    Logger::Get().EnableLog(LogSeverity::ALL);

    WindowDescriptor window_desc;
    window_desc.MSAA_samples = 4;
    window_desc.height = 1000;
    window_desc.width = 1000;

    Window* window = CreateWindow(window_desc);
    RHI_Device* device = CreateDevice(window);
    input::SetActiveWindow(static_cast<GLFWwindow*>(window->GetNativeWindow()));

    ShaderDescriptor shader_desc;
    shader_desc.fragment_source = "basic.frag";
    shader_desc.vertex_source = "basic.vert";
    ShaderHandle basic_shader = device->CreateShader(shader_desc);

    ShaderDescriptor screen_shader_desc;
    screen_shader_desc.fragment_source = "screen.frag";
    screen_shader_desc.vertex_source = "screen.vert";
    ShaderHandle screen_shader = device->CreateShader(screen_shader_desc);

    BufferDescriptor ebo_descriptor;
    ebo_descriptor.buffer_usage = BufferUsage::Static;
    ebo_descriptor.data = indices.data();
    ebo_descriptor.map_flags = map_flags::Read;
    ebo_descriptor.size = sizeof(indices);
    BufferHandle EBO = device->CreateElementBuffer(ebo_descriptor);

    BufferDescriptor vbo_descriptor;
    vbo_descriptor.buffer_usage = BufferUsage::Dynamic;
    vbo_descriptor.data = verts.data();
    vbo_descriptor.map_flags = map_flags::Write;
    vbo_descriptor.size = bytesize(verts);
    BufferHandle VBO = device->CreateVertexBuffer(vbo_descriptor);

    std::vector<VertexAttribute> vbo_attrs(2);
    auto& vbo_pos = vbo_attrs[0];
    vbo_pos.format = DataFormat::Float3;
    vbo_pos.relative_offset = offsetof(Vertex, position);
    vbo_pos.shader_location = LUM_LAYOUT_POSITION;
    auto& vbo_color = vbo_attrs[1];
    vbo_color.format = DataFormat::Float3;
    vbo_color.relative_offset = offsetof(Vertex, color);
    vbo_color.shader_location = LUM_LAYOUT_COLOR;
    VertexLayoutDescriptor vao_descriptor;
    vao_descriptor.attributes = vbo_attrs;
    vao_descriptor.stride = sizeof(Vertex);
    VertexLayoutHandle VAO = device->CreateVertexLayout(vao_descriptor, VBO);

    device->AttachElementBufferToLayout(EBO, VAO);

    FramebufferTextureDescriptor framebuffer_tex_desc;
    framebuffer_tex_desc.attachment = FramebufferAttachment::Color_Attachment;
    framebuffer_tex_desc.width = window->GetWidth();
    framebuffer_tex_desc.height = window->GetHeight();
    TextureHandle FBO_TEX = device->CreateFramebufferTexture(framebuffer_tex_desc);
    FramebufferHandle FBO = device->CreateFramebuffer(FramebufferDescriptor{});
    device->BindFramebuffer(FBO);
    device->SetFramebufferColorTexture(FBO, FBO_TEX, 0);

    BufferDescriptor FBO_VBO_desc;
    FBO_VBO_desc.buffer_usage = BufferUsage::Dynamic;
    FBO_VBO_desc.map_flags = map_flags::Write;
    FBO_VBO_desc.size = sizeof(quad);
    FBO_VBO_desc.data = quad;
    BufferHandle FBO_VBO = device->CreateVertexBuffer(FBO_VBO_desc);
    
    std::vector<VertexAttribute> at(2);
    auto& pos = at[0];
    pos.format = DataFormat::Float2;
    pos.relative_offset = 0;
    pos.shader_location = 0;
    auto& uv = at[1];
    uv.format = DataFormat::Float2;
    uv.relative_offset = 2 * sizeof(float);
    uv.shader_location = 1;
    VertexLayoutDescriptor FBO_VAO_desc;
    FBO_VAO_desc.stride = 4 * sizeof(float);
    FBO_VAO_desc.attributes = at;
    VertexLayoutHandle FBO_VAO = device->CreateVertexLayout(FBO_VAO_desc, FBO_VBO);

    while (window->IsOpen()) {
        device->BeginFrame();

        device->BindFramebuffer(FBO);
        device->BindShader(basic_shader);
        device->DrawElements(VAO, indices.size());
        device->UnbindFramebuffer();

        device->BindShader(screen_shader);
        device->BindTexture(FBO_TEX, 7);
        device->Draw(FBO_VAO, 6);
        

        device->EndFrame();
    }
}
/*
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
    std::vector<VertexAttribute> attrib(2);
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

    ShaderDescriptor shaderdesc;
    shaderdesc.fragment_source = "screen.frag";
    shaderdesc.vertex_source = "screen.vert";
    auto screenshader = device->CreateShader(shaderdesc);

    /*
    GLuint quadVBO, quadVAO;
    
    glCreateVertexArrays(1, &quadVAO);
    
    glCreateBuffers(1, &quadVBO);
    
    glNamedBufferData(quadVBO, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(quadVAO, 0, quadVBO, 0, 4 * sizeof(float));

    glEnableVertexArrayAttrib(quadVAO, 0);
    glVertexArrayAttribFormat(quadVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(quadVAO, 0, 0);

    glEnableVertexArrayAttrib(quadVAO, 1);
    glVertexArrayAttribFormat(quadVAO, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
    glVertexArrayAttribBinding(quadVAO, 1, 0);

        GLuint fbo, fbot, depth;
    glCreateFramebuffers(1, &fbo);
    glCreateTextures(GL_TEXTURE_2D, 1, &fbot);
    glCreateTextures(GL_TEXTURE_2D, 1, &depth);
    glTextureStorage2D(fbot, 1, GL_RGB8, window->GetWidth(), window->GetHeight());
    glTextureStorage2D(depth, 1, GL_DEPTH_COMPONENT24, window->GetWidth(), window->GetHeight());

    glFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, depth, 0);
    glFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, fbot, 0);

    float quad[] = {
        // pos      // uv
        -1, -1,     0, 0,
         1, -1,     1, 0,
         1,  1,     1, 1,

        -1, -1,     0, 0,
         1,  1,     1, 1,
        -1,  1,     0, 1,
    };

    BufferDescriptor quadvbodesc;
    quadvbodesc.buffer_usage = BufferUsage::Dynamic;
    quadvbodesc.map_flags = map_flags::Write;
    quadvbodesc.size = sizeof(quad);
    quadvbodesc.data = quad;

    auto quadVbo = device->CreateVertexBuffer(quadvbodesc);

    std::vector<VertexAttribute> at(1);
    auto& pos = at[0];
    pos.format = DataFormat::Float2;
    pos.relative_offset = 0;
    pos.shader_location = 0;

    auto& uv = at[1];
    uv.format = DataFormat::Float2;
    pos.relative_offset = 2 * sizeof(float);
    pos.shader_location = 1;

    VertexLayoutDescriptor quadvaodesc;

    quadvaodesc.stride = 4 * sizeof(float);
    quadvaodesc.attributes = at;

    auto quadVao = device->CreateVertexLayout(quadvaodesc, quadVbo);

    FramebufferTextureDescriptor tfbodesc;
    tfbodesc.width = window->GetWidth();
    tfbodesc.height = window->GetHeight();

    auto fbocoltexture = device->CreateFramebufferTexture(tfbodesc);

    FramebufferDescriptor fbodesc;
    auto fbo = device->CreateFramebuffer(fbodesc);

    device->SetFramebufferColorTexture(fbo, fbocoltexture, 0);






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

        device->BindFramebuffer(fbo);
        glClear(GL_COLOR_BUFFER_BIT);
        device->BindShader(shader);
        cubo.view = c.view;
        cubo.proj = c.projection;
        device->UpdateBuffer(model_ubo, &UBO_model, 0, 0);
        device->UpdateBuffer(ubo, &cubo, 0, 0);

        device->BindSampler(sampler);
        device->BindTexture(texture);
        device->DrawElements(vao, indices.size());

        device->UnbindFramebuffer();

        glBindVertexArray(quadVAO);
        device->BindShader(screenshader);
        glBindTextureUnit(7, fbot);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        device->EndFrame();
    }

}
*/