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
                glfwSetInputMode(static_cast<GLFWwindow*>(m_window->get_native_window()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else glfwSetInputMode(static_cast<GLFWwindow*>(m_window->get_native_window()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

    }

    void RecalculateMVP() {
        aspect_ratio = (float32)m_window->get_width() / (float32)m_window->get_height();
        view = glm::lookAt(position, front + position, up);
        projection = glm::perspective(glm::radians(fov), aspect_ratio, min_plane, max_plane);

    }

    void Init(Window* wind) {
        aspect_ratio = (double)wind->get_width() / (double)wind->get_height();
        glm::vec2 mouse_pos = input::GetMousePos();
        lastX = mouse_pos.x;
        lastY = mouse_pos.y;
    }
    
    float32 move_speed = 0.07f;
    float32 aspect_ratio = 0;
    float32 lastX = 0;
    float32 lastY = 0;
    float32 yaw = 0, pitch = 0;
    float32 fov = 45.f;
    float32 min_plane = 0.1f;
    float32 max_plane = 1000.f;
    float32 sensivity = 0.1f;
    bool mouse_locked = false;

    bool saveMousePos = false;
    bool mouseLocked = true;

};

LUM_UNIFORM_BUFFER_STRUCT CameraUBO {
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 pos;
};
LUM_UNIFORM_BUFFER_STRUCT ModelUBO {
    glm::mat4 model;
};

glm::vec3 model_pos = { 0,0,0 };
glm::vec3 model_rot = { 0,0,0 };
glm::vec3 model_scale = { 1,1,1 };

void UpdateCamera(RenderDevice* dev, Camera& cam, rhi::BufferHandle& ubo, CameraUBO& camstruct, rhi::BufferHandle& modelubo, ModelUBO modelstruct) {
    
    cam.Update();

    glm::quat rot = glm::quat(glm::radians(model_rot));
    glm::mat4 rotation = glm::mat4_cast(rot);
    modelstruct.model = glm::mat4(1.f);
    modelstruct.model = glm::translate(modelstruct.model, model_pos);
    modelstruct.model = modelstruct.model * rotation;
    modelstruct.model = glm::scale(modelstruct.model, model_scale);
    
    camstruct.proj = cam.projection;
    camstruct.view = cam.view;
    camstruct.pos = cam.position;

    dev->UpdateBuffer(ubo, &camstruct, 0, 0);
    dev->UpdateBuffer(modelubo, &modelstruct, 0, 0);
    
}

std::vector<Vertex> skyboxVerts = {
    {{-1.0f,  1.0f, -1.0f}}, {{-1.0f, -1.0f, -1.0f}},
    {{ 1.0f, -1.0f, -1.0f}}, {{ 1.0f, -1.0f, -1.0f}},
    {{ 1.0f,  1.0f, -1.0f}}, {{-1.0f,  1.0f, -1.0f}},

    {{-1.0f, -1.0f,  1.0f}}, {{-1.0f, -1.0f, -1.0f}},
    {{-1.0f,  1.0f, -1.0f}}, {{-1.0f,  1.0f, -1.0f}},
    {{-1.0f,  1.0f,  1.0f}}, {{-1.0f, -1.0f,  1.0f}},

    {{ 1.0f, -1.0f, -1.0f}}, {{ 1.0f, -1.0f,  1.0f}},
    {{ 1.0f,  1.0f,  1.0f}}, {{ 1.0f,  1.0f,  1.0f}},
    {{ 1.0f,  1.0f, -1.0f}}, {{ 1.0f, -1.0f, -1.0f}},

    {{-1.0f, -1.0f,  1.0f}}, {{-1.0f,  1.0f,  1.0f}},
    {{ 1.0f,  1.0f,  1.0f}}, {{ 1.0f,  1.0f,  1.0f}},
    {{ 1.0f, -1.0f,  1.0f}}, {{-1.0f, -1.0f,  1.0f}},

    {{-1.0f,  1.0f, -1.0f}}, {{ 1.0f,  1.0f, -1.0f}},
    {{ 1.0f,  1.0f,  1.0f}}, {{ 1.0f,  1.0f,  1.0f}},
    {{-1.0f,  1.0f,  1.0f}}, {{-1.0f,  1.0f, -1.0f}},

    {{-1.0f, -1.0f, -1.0f}}, {{-1.0f, -1.0f,  1.0f}},
    {{ 1.0f, -1.0f, -1.0f}}, {{ 1.0f, -1.0f, -1.0f}},
    {{-1.0f, -1.0f,  1.0f}}, {{ 1.0f, -1.0f,  1.0f}}
};
std::vector<Vertex> cubeVerts = {
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
std::vector<uint32> cubeIndices = {
    0,1,2, 2,3,0,         // front
    4,5,6, 6,7,4,         // back
    8,9,10, 10,11,8,      // left
    12,13,14, 14,15,12,   // right
    16,17,18, 18,19,16,   // top
    20,21,22, 22,23,20    // bottom
};

RenderDevice* device = nullptr;

auto CreateShader(ccharptr vert, ccharptr frag) {
    return device->CreateShader({vert, frag});
}
auto CreateCubeEBO() {
    BufferDescriptor cubeEBODesc;
    cubeEBODesc.bufferUsage = BufferUsage::Static;
    cubeEBODesc.data = cubeIndices.data();
    cubeEBODesc.size = bytesize(cubeIndices);
    return device->CreateElementBuffer(cubeEBODesc);
}
auto CreateCubeVAO(auto vbo) {
    std::vector<VertexAttribute> attributes(3);
    attributes[0].format = DataFormat::Vec3;
    attributes[0].relativeOffset = offsetof(Vertex, position);
    attributes[0].shaderLocation = LUM_LAYOUT_POSITION;

    attributes[1].format = DataFormat::Vec3;
    attributes[1].relativeOffset = offsetof(Vertex, color);
    attributes[1].shaderLocation = LUM_LAYOUT_COLOR;

    attributes[2].format = DataFormat::Vec2;
    attributes[2].relativeOffset = offsetof(Vertex, uv);
    attributes[2].shaderLocation = LUM_LAYOUT_UV;

    VertexLayoutDescriptor cubeVAODesc;
    cubeVAODesc.stride = sizeof(Vertex);
    cubeVAODesc.attributes = attributes;
    return device->CreateVertexLayout(cubeVAODesc, vbo);
}
auto CreateCubeVBO() {
    BufferDescriptor cubeVBODesc;
    cubeVBODesc.bufferUsage = BufferUsage::Dynamic;
    cubeVBODesc.data = cubeVerts.data();
    cubeVBODesc.mapFlags = Mapflag::Write;
    cubeVBODesc.size = bytesize(cubeVerts);
    return device->CreateVertexBuffer(cubeVBODesc);
}
auto CreateCubeTexture() {
    TextureDescriptor textureDesc;
    textureDesc.filename = "glass2.jpg";
    auto texture = device->CreateTexture2D(textureDesc);
    device->SetTextureBinding(texture, LUM_TEXTURE_BINDING_01);
    return texture;
}
auto CreateCubePipeline(auto shader) {
    PipelineDescriptor cubePipeline;
    cubePipeline.mBlend.bEnabled = true;
    cubePipeline.mBlend.dstAlphaFactor = BlendFactor::SrcAlpha;
    cubePipeline.mBlend.srcAlphaFactor = BlendFactor::SrcAlpha;
    cubePipeline.mBlend.dstColorFactor = BlendFactor::SrcAlpha;
    cubePipeline.mBlend.srcColorFactor = BlendFactor::SrcAlpha;
    cubePipeline.mBlend.alphaOp = BlendOp::Max;
    cubePipeline.mBlend.colorOp = BlendOp::Max;

    cubePipeline.mDepthStencil.depth.bEnabled = true;
    cubePipeline.mDepthStencil.depth.bWriteToZBuffer = false;
    cubePipeline.mDepthStencil.depth.compareFlag = CompareFlag::Less;

    //cubePipeline.rasterizer.topologyMode = TopologyMode::Line;

    //cubePipeline.cull.bEnabled = true;

    cubePipeline.shader = shader;
    return device->CreatePipeline(cubePipeline);
}

auto CreateSkyboxVAO(auto vbo) {
    VertexLayoutDescriptor vaoDesc;
    std::vector<VertexAttribute> attr(1);
    attr[0].format = DataFormat::Vec3;
    attr[0].relativeOffset = offsetof(Vertex, position);
    attr[0].shaderLocation = LUM_LAYOUT_POSITION;

    vaoDesc.attributes = attr;
    vaoDesc.stride = sizeof(Vertex);
    return device->CreateVertexLayout(vaoDesc, vbo);
}
auto CreateSkyboxVBO() {
    BufferDescriptor vboDesc;
    vboDesc.bufferUsage = BufferUsage::Static;
    vboDesc.data = skyboxVerts.data();
    vboDesc.size = bytesize(skyboxVerts);
    return device->CreateVertexBuffer(vboDesc);
}
auto CreateSkyboxTexture() {
    TextureCubemapDescriptor cubemapTexDesc;
    cubemapTexDesc.faces[LUM_CUBEMAP_NEGATIVE_X] = "nx.png";
    cubemapTexDesc.faces[LUM_CUBEMAP_NEGATIVE_Y] = "ny.png";
    cubemapTexDesc.faces[LUM_CUBEMAP_NEGATIVE_Z] = "nz.png";
    cubemapTexDesc.faces[LUM_CUBEMAP_POSITIVE_X] = "px.png";
    cubemapTexDesc.faces[LUM_CUBEMAP_POSITIVE_Y] = "py.png";
    cubemapTexDesc.faces[LUM_CUBEMAP_POSITIVE_Z] = "pz.png";
    return device->CreateCubemapTexture(cubemapTexDesc);
}
auto CreateSkyboxPipeline(auto shader) {
    PipelineDescriptor skyboxPipeline;
    skyboxPipeline.mDepthStencil.depth.bEnabled = true;
    skyboxPipeline.mDepthStencil.depth.bWriteToZBuffer = false;
    skyboxPipeline.mDepthStencil.depth.compareFlag = CompareFlag::LessEqual;

    skyboxPipeline.mCull.bEnabled = true;

    skyboxPipeline.shader = shader;
    return device->CreatePipeline(skyboxPipeline);
}

float32 quad[] = {
    // pos      // uv
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,

    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f
};
int main() {
    //Logger::Get().enable_log(LogSeverity::Info);
    Logger::Get().disable_log(LogSeverity::Debug);
    WindowDescriptor windowDesc;
    windowDesc.msaaSamples = 4;
    windowDesc.bFullscreen = false;
    windowDesc.height = 920;
    windowDesc.width = 1280;

    Window* window = CreateWindow(windowDesc);
    device = CreateDevice(window);
    input::SetActiveWindow(static_cast<GLFWwindow*>(window->get_native_window()));

    auto basicShader = CreateShader("basic.vert", "basic.frag");
    auto skyboxShader = CreateShader("skybox_pass.vert", "skybox_pass.frag");
    
    auto cubeVBO = CreateCubeVBO();
    auto cubeEBO = CreateCubeEBO();
    auto cubeVAO = CreateCubeVAO(cubeVBO);
    auto cubeTexture = CreateCubeTexture();
    device->AttachElementBufferToLayout(cubeEBO, cubeVAO);

    auto skyVBO = CreateSkyboxVBO();
    auto skyVAO = CreateSkyboxVAO(skyVBO);
    auto skyboxTexture = CreateSkyboxTexture();

    Camera c{ window };
    CameraUBO camUBO{};
    ModelUBO mUBO{};

    BufferDescriptor uboDesc;
    uboDesc.bufferUsage = BufferUsage::Dynamic;
    uboDesc.mapFlags = Mapflag::Write;
    uboDesc.size = sizeof(CameraUBO);
    auto cameraUBO = device->CreateUniformBuffer(uboDesc);

    BufferDescriptor muboDesc;
    muboDesc.bufferUsage = BufferUsage::Dynamic;
    muboDesc.mapFlags = Mapflag::Write;
    muboDesc.size = sizeof(ModelUBO);
    auto modelUBO = device->CreateUniformBuffer(muboDesc);

    device->SetUniformBufferBinding(cameraUBO, LUM_UBO_CAMERA_BINDING);
    device->SetUniformBufferBinding(modelUBO, LUM_UBO_MODEL_BINDING);

    SamplerDescriptor sdesc;
    sdesc.magFilter = SamplerMagFilter::Linear;
    sdesc.minFilter = SamplerMinFilter::LinearMipmapLinear;
    sdesc.anisotropy = 16;
    sdesc.wrapS = SamplerWrap::ClampEdge;
    sdesc.wrapT = SamplerWrap::ClampEdge;
    sdesc.wrapR = SamplerWrap::ClampEdge;

    auto sampler = device->CreateSampler(sdesc);

    auto cubePip = CreateCubePipeline(basicShader);
    auto skyboxPip = CreateSkyboxPipeline(skyboxShader);
    
    do {

        UpdateCamera(device, c, cameraUBO, camUBO, modelUBO, mUBO);
        c.Update();

        int x, y;
        glfwGetWindowSize(static_cast<GLFWwindow*>(window->get_native_window()), &x, &y);
        window->set_width(x);
        window->set_height(y);

        device->BeginFrame();
        
        device->BindPipeline(cubePip);
        device->BindSampler(sampler, LUM_TEXTURE_BINDING_01);
        device->BindTexture(cubeTexture);
        //device->SetColorMask(false, false, false, false);
        device->DrawElements(cubeVAO, cubeIndices.size());

        device->BindPipeline(skyboxPip);
        device->BindSampler(sampler, LUM_CUBEMAP_BINDING);
        device->BindTexture(skyboxTexture, LUM_CUBEMAP_BINDING);
        device->Draw(skyVAO, skyboxVerts.size());

        device->EndFrame();

    } while (false);

    device->GetProfilerInfo();

}