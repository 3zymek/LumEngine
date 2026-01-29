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
#include "core/flags.hpp"
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
    
    float32 move_speed = 0.1f;
    float32 aspect_ratio = 0;
    float32 lastX = 0;
    float32 lastY = 0;
    float32 yaw = 0, pitch = 0;
    float32 fov = 45.f;
    float32 min_plane = 0.1f;
    float32 max_plane = 1000.f;
    float32 sensivity = 0.2f;
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

glm::vec3 model_pos = { 0,0,0 };
glm::vec3 model_rot = { 0,0,0 };
glm::vec3 model_scale = { 1,1,1, };

void UpdateCamera(RenderDevice* dev, Camera& cam, rhi::BufferHandle& ubo, CameraUBO& camstruct, rhi::BufferHandle& modelubo, ModelUBO modelstruct) {
    
    glm::quat rot = glm::quat(glm::radians(model_rot));
    glm::mat4 rotation = glm::mat4_cast(rot);
    modelstruct.model = glm::mat4(1.f);
    modelstruct.model = glm::translate(modelstruct.model, model_pos);
    modelstruct.model = modelstruct.model * rotation;
    modelstruct.model = glm::scale(modelstruct.model, model_scale);
    
    camstruct.proj = cam.projection;
    camstruct.view = cam.view;

    dev->UpdateBuffer(ubo, &camstruct, 0, 0);
    dev->UpdateBuffer(modelubo, &modelstruct, 0, 0);
    
    cam.Update();
}

std::vector<Vertex> verts = {
    // Front face (z = 0.5)
    {{-0.5, -0.5,  0.5}, {0, 0, 1}, {0, 0}},
    {{ 0.5, -0.5,  0.5}, {0, 0, 1}, {1, 0}},
    {{ 0.5,  0.5,  0.5}, {0, 0, 1}, {1, 1}},
    {{-0.5,  0.5,  0.5}, {0, 0, 1}, {0, 1}},

    // Back face (z = -0.5)
    {{-0.5, -0.5, -0.5}, {0, 0, -1}, {1, 0}},
    {{-0.5,  0.5, -0.5}, {0, 0, -1}, {1, 1}},
    {{ 0.5,  0.5, -0.5}, {0, 0, -1}, {0, 1}},
    {{ 0.5, -0.5, -0.5}, {0, 0, -1}, {0, 0}},

    // Left face (x = -0.5)
    {{-0.5, -0.5, -0.5}, {-1, 0, 0}, {0, 0}},
    {{-0.5, -0.5,  0.5}, {-1, 0, 0}, {1, 0}},
    {{-0.5,  0.5,  0.5}, {-1, 0, 0}, {1, 1}},
    {{-0.5,  0.5, -0.5}, {-1, 0, 0}, {0, 1}},

    // Right face (x = 0.5)
    {{ 0.5, -0.5, -0.5}, {1, 0, 0}, {1, 0}},
    {{ 0.5,  0.5, -0.5}, {1, 0, 0}, {1, 1}},
    {{ 0.5,  0.5,  0.5}, {1, 0, 0}, {0, 1}},
    {{ 0.5, -0.5,  0.5}, {1, 0, 0}, {0, 0}},

    // Top face (y = 0.5)
    {{-0.5,  0.5,  0.5}, {0, 1, 0}, {0, 0}},
    {{ 0.5,  0.5,  0.5}, {0, 1, 0}, {1, 0}},
    {{ 0.5,  0.5, -0.5}, {0, 1, 0}, {1, 1}},
    {{-0.5,  0.5, -0.5}, {0, 1, 0}, {0, 1}},

    // Bottom face (y = -0.5)
    {{-0.5, -0.5,  0.5}, {0, -1, 0}, {1, 0}},
    {{-0.5, -0.5, -0.5}, {0, -1, 0}, {1, 1}},
    {{ 0.5, -0.5, -0.5}, {0, -1, 0}, {0, 1}},
    {{ 0.5, -0.5,  0.5}, {0, -1, 0}, {0, 0}}
};

std::vector<uint32> indices = {
    0,1,2, 2,3,0,         // front
    4,5,6, 6,7,4,         // back
    8,9,10, 10,11,8,      // left
    12,13,14, 14,15,12,   // right
    16,17,18, 18,19,16,   // top
    20,21,22, 22,23,20    // bottom
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

    // =====================
    // Logger + Window + Device
    // =====================
    Logger::Get().EnableLog(LogSeverity::ALL);

    WindowDescriptor window_desc;
    window_desc.MSAA_samples = 4;
    window_desc.height = 1000;
    window_desc.width = 2000;

    Window* window = CreateWindow(window_desc);
    RenderDevice* device = create_device(window);
    input::SetActiveWindow(static_cast<GLFWwindow*>(window->GetNativeWindow()));

    // =====================
    // Shaders
    // =====================
    ShaderDescriptor shader_desc;
    shader_desc.fragment_source = "basic.frag";
    shader_desc.vertex_source = "basic.vert";
    ShaderHandle basic_shader = device->CreateShader(shader_desc);

    ShaderDescriptor screen_shader_desc;
    screen_shader_desc.fragment_source = "screen.frag";
    screen_shader_desc.vertex_source = "screen.vert";
    ShaderHandle screen_shader = device->CreateShader(screen_shader_desc);

    // =====================
    // Buffers (VBO, EBO)
    // =====================
    BufferDescriptor vbo_descriptor;
    vbo_descriptor.bufferUsage = BufferUsage::Dynamic;
    vbo_descriptor.data = verts.data();
    vbo_descriptor.mapFlags = Mapflag::Write;
    vbo_descriptor.size = bytesize(verts);
    BufferHandle VBO = device->CreateVertexBuffer(vbo_descriptor);

    BufferDescriptor ebo_descriptor;
    ebo_descriptor.bufferUsage = BufferUsage::Dynamic;
    ebo_descriptor.data = indices.data();
    ebo_descriptor.mapFlags = Mapflag::Read;
    ebo_descriptor.size = bytesize(indices);
    BufferHandle EBO = device->CreateElementBuffer(ebo_descriptor);

    // =====================
    // Vertex Layout / VAO
    // =====================
    std::vector<VertexAttribute> vbo_attrs(3);
    vbo_attrs[0].format = DataFormat::Vec3;
    vbo_attrs[0].relativeOffset = offsetof(Vertex, position);
    vbo_attrs[0].shaderLocation = LUM_LAYOUT_POSITION;

    vbo_attrs[1].format = DataFormat::Vec3;
    vbo_attrs[1].relativeOffset = offsetof(Vertex, color);
    vbo_attrs[1].shaderLocation = LUM_LAYOUT_COLOR;

    vbo_attrs[2].format = DataFormat::Vec2;
    vbo_attrs[2].relativeOffset = offsetof(Vertex, uv);
    vbo_attrs[2].shaderLocation = LUM_LAYOUT_UV;

    VertexLayoutDescriptor vao_descriptor;
    vao_descriptor.attributes = vbo_attrs;
    vao_descriptor.stride = sizeof(Vertex);
    VertexLayoutHandle VAO = device->CreateVertexLayout(vao_descriptor, VBO);
    device->AttachElementBufferToLayout(EBO, VAO);

    // =====================
    // FBO
    // =====================
    FramebufferTextureDescriptor framebuffer_tex_desc;
    framebuffer_tex_desc.attachment = FramebufferAttachment::ColorAttach;
    framebuffer_tex_desc.width = window->GetWidth();
    framebuffer_tex_desc.height = window->GetHeight();
    TextureHandle FBO_TEX = device->CreateFramebufferTexture(framebuffer_tex_desc);

    FramebufferHandle FBO = device->CreateFramebuffer();
    device->BindFramebuffer(FBO);
    device->SetFramebufferColorTexture(FBO, FBO_TEX, 0);

    FramebufferTextureDescriptor framebuffer_depth_desc;
    framebuffer_depth_desc.attachment = FramebufferAttachment::DepthAttach;
    framebuffer_depth_desc.width = window->GetWidth();
    framebuffer_depth_desc.height = window->GetHeight();
    auto FBO_DEPTH = device->CreateFramebufferTexture(framebuffer_depth_desc);
    device->SetFramebufferDepthTexture(FBO, FBO_DEPTH);

    // FBO quad
    BufferDescriptor FBO_VBO_desc;
    FBO_VBO_desc.bufferUsage = BufferUsage::Dynamic;
    FBO_VBO_desc.mapFlags = Mapflag::Write;
    FBO_VBO_desc.size = sizeof(quad);
    FBO_VBO_desc.data = quad;
    BufferHandle FBO_VBO = device->CreateVertexBuffer(FBO_VBO_desc);

    std::vector<VertexAttribute> at(2);
    at[0].format = DataFormat::Vec2;
    at[0].relativeOffset = 0;
    at[0].shaderLocation = 0;
    at[1].format = DataFormat::Vec2;
    at[1].relativeOffset = 2 * sizeof(float);
    at[1].shaderLocation = 1;

    VertexLayoutDescriptor FBO_VAO_desc;
    FBO_VAO_desc.stride = 4 * sizeof(float);
    FBO_VAO_desc.attributes = at;
    VertexLayoutHandle FBO_VAO = device->CreateVertexLayout(FBO_VAO_desc, FBO_VBO);

    // =====================
    // Pipelines
    // =====================
    PipelineDescriptor pipeline_desc;
    pipeline_desc.rasterizer.topologyMode = PolygonMode::Fill;

    pipeline_desc.depthStencil.depth.bEnabled = true;
    pipeline_desc.depthStencil.depth.bWriteToZBuffer = true;
    pipeline_desc.depthStencil.depth.compareFlag = CompareFlag::Less;
    
    //pipeline_desc.blend.bEnabled = true;
    pipeline_desc.blend.srcAlphaFactor = BlendFactor::SrcAlpha;
    pipeline_desc.blend.srcColorFactor = BlendFactor::SrcAlpha;
    pipeline_desc.blend.dstColorFactor = BlendFactor::OneMinusSrcColor;
    pipeline_desc.blend.dstAlphaFactor = BlendFactor::OneMinusSrcColor;
    pipeline_desc.blend.alphaOp = BlendOp::Max;
    pipeline_desc.blend.colorOp = BlendOp::Max;

    //pipeline_desc.cull.bEnabled = true;
    pipeline_desc.cull.face = Face::Back;
    pipeline_desc.cull.windingOrder = WindingOrder::CounterClockwise;

    pipeline_desc.scissor.bEnabled = true;
    
    auto debug_pipeline = device->CreatePipeline(pipeline_desc);
    


    PipelineDescriptor pipeline_desc2;
    pipeline_desc2.rasterizer.topologyMode = PolygonMode::Line;
    
    pipeline_desc2.depthStencil.depth.bEnabled = true;
    pipeline_desc2.depthStencil.depth.bWriteToZBuffer = true;
    pipeline_desc2.depthStencil.depth.compareFlag = CompareFlag::Less;
    
    pipeline_desc2.blend.bEnabled = true;

    pipeline_desc2.scissor.bEnabled = true;
    
    auto debug_pipeline2 = device->CreatePipeline(pipeline_desc2);
    auto basic_pipeline = device->CreatePipeline(PipelineDescriptor{});
    bool topologyChanged = false;

    // =====================
    // Camera + UBOs
    // =====================
    Camera c{ window };

    CameraUBO camera_ubo_struct{};
    BufferDescriptor camera_ubo_desc;
    camera_ubo_desc.bufferUsage = BufferUsage::Dynamic;
    camera_ubo_desc.mapFlags = Mapflag::Write;
    camera_ubo_desc.size = sizeof(CameraUBO);
    auto cameraUniform = device->CreateUniformBuffer(camera_ubo_desc);

    ModelUBO model_ubo_struct{};
    BufferDescriptor model_ubo_desc;
    model_ubo_desc.bufferUsage = BufferUsage::Dynamic;
    model_ubo_desc.mapFlags = Mapflag::Write;
    model_ubo_desc.size = sizeof(CameraUBO);
    auto modelUniform = device->CreateUniformBuffer(model_ubo_desc);

    device->SetUniformBufferBinding(cameraUniform, LUM_UBO_CAMERA_BINDING);
    device->SetUniformBufferBinding(modelUniform, LUM_UBO_MODEL_BINDING);

    // =====================
    // Textures + Samplers
    // =====================
    TextureDescriptor grass_desc;
    grass_desc.filename = "stone.png";
    grass_desc.height = 100;
    grass_desc.width = 100;
    auto grassTexture = device->CreateTexture2D(grass_desc);

    SamplerDescriptor grass_sampler_desc;
    grass_sampler_desc.magFilter = SamplerMagFilter::Nearest;
    grass_sampler_desc.minFilter = SamplerMinFilter::Nearest;
    auto grassSampler = device->CreateSampler(grass_sampler_desc);
    device->SetSamplerBinding(grassSampler, LUM_TEXTURE_BINDING_01);
    device->SetTextureBinding(grassTexture, LUM_TEXTURE_BINDING_01);

    // =====================
    // Render loop
    // =====================
    while (window->IsOpen()) {
        device->BeginFrame();

        // ---- ImGui Transform
        ImGui::Begin("Transform");
        ImGui::DragFloat3("position", glm::value_ptr(model_pos), 0.1f, -1000, 1000);
        ImGui::DragFloat3("scale", glm::value_ptr(model_scale), 0.1f, -1000, 1000);
        ImGui::DragFloat3("rotation", glm::value_ptr(model_rot), 0.1f, -1000, 1000);
        ImGui::End();

        // ---- Camera update
        device->BindShader(basic_shader);
        UpdateCamera(device, c, cameraUniform, camera_ubo_struct, modelUniform, model_ubo_struct);
        
        // ---- Render FBO
        device->BindFramebuffer(FBO);
        device->ClearFramebuffer(FBO, { 0.f,0.f,0.f,1.f }, 1.0f);
        device->BindPipeline(topologyChanged ? debug_pipeline2 : debug_pipeline);
        //device->SetScissor(0, 0, window->GetWidth() / 2.f, window->GetHeight() / 2.f);
        device->BindSampler(grassSampler);
        device->BindTexture(grassTexture);

        device->EnableDepthWrite(false);

        device->DrawElements(VAO, indices.size());
        
        device->EnableDepthWrite(true);

        if (input::KeyPressedOnce(input::Key::SPACE))
            topologyChanged = !topologyChanged;


        device->UnbindFramebuffer();

        device->BindPipeline(basic_pipeline);
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