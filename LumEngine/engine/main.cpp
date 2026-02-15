#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "window_context/input_common.hpp"
#include "core/utils/logger.hpp"
#include "editor.hpp"
#include "audio/components/c_audio_listener.hpp"
#include "window_context/window.hpp"
#include "rhi/core/rhi_device.hpp"
#include "core/utils/asset_service.hpp"
#include "rhi/rhi_common.hpp"
#include "core/shaders_define.h"
#include "core/math/backend/gtx/string_cast.hpp"
#include "imgui.h"
#include "core/utils/flags.hpp"
#include "core/utils/fixed_string.hpp"
#include "assimp/Importer.hpp"
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
    float32 move_speed = 0.07f;
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
        aspect_ratio = (float32)m_window->GetWidth() / (float32)m_window->GetHeight();
        view = glm::lookAt(position, front + position, up);
        projection = glm::perspective(glm::radians(fov), aspect_ratio, min_plane, max_plane);

    }

    void Init(Window* wind) {
        aspect_ratio = (double)wind->GetWidth() / (double)wind->GetHeight();
        glm::vec2 mouse_pos = input::GetMousePos();
        lastX = mouse_pos.x;
        lastY = mouse_pos.y;
    }
    
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

RenderDevice* device = nullptr;

struct BaseObject {

    math::Vec3 modelPos = { 0, 0, 0 };
    math::Vec3 modelRot = { 0,0,0 };
    math::Vec3 modelScale = { 1, 1, 1 };

    TextureHandle texture{};
    BufferHandle VBO{};
    BufferHandle EBO{};
    VertexLayoutHandle VAO{};

    void Draw(uint16 texbinding, uint32 indices) {

        device->BindTexture(texture, texbinding);
        device->DrawElements(VAO, indices);

    }

};

void UpdateCamera(RenderDevice* dev, Camera& cam, const rhi::BufferHandle& ubo, CameraUBO& camstruct, rhi::BufferHandle& modelubo, BaseObject obj) {
    
    cam.Update();

    glm::quat rot = glm::quat(glm::radians(obj.modelRot));
    glm::mat4 rotation = glm::mat4_cast(rot);
    math::Mat4 model = math::Mat4(1.f);
    model = glm::mat4(1.f);
    model = glm::translate(model, obj.modelPos);
    model = model * rotation;
    model = glm::scale(model, obj.modelScale);
    
    camstruct.proj = cam.projection;
    camstruct.view = cam.view;
    camstruct.pos = cam.position;

    dev->UpdateBuffer(ubo, &camstruct, 0, 0);
    dev->UpdateBuffer(modelubo, &model, 0, 0);
    
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
std::vector<uint32> skyboxIndices = {
    0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
};

std::vector<Vertex> cubeVerts = {
    { { -500, 0, -500 }, {0,1,0}, {0,0} }, // 0
    { {  500, 0, -500 }, {0,1,0}, {1,0} }, // 1
    { {  500, 0,  500 }, {0,1,0}, {1,1} }, // 2
    { { -500, 0,  500 }, {0,1,0}, {0,1} }  // 3
};
std::vector<uint32> cubeIndices = {
    0, 1, 2,
    0, 2, 3
};

auto CreateShader(ccharptr vert, ccharptr frag) {
    return device->CreateShader({vert, frag});
}
auto CreateCubeEBO() {
    BufferDescriptor cubeEBODesc;
    cubeEBODesc.mBufferUsage = BufferUsage::Static;
    cubeEBODesc.mData = cubeIndices.data();
    cubeEBODesc.mSize = ByteSize(cubeIndices);
    return device->CreateElementBuffer(cubeEBODesc);
}
auto CreateCubeVAO(auto vbo) {
    std::vector<VertexAttribute> attributes(3);
    attributes[0].mFormat = DataFormat::Vec3;
    attributes[0].mRelativeOffset = offsetof(Vertex, position);
    attributes[0].mShaderLocation = LUM_LAYOUT_POSITION;

    attributes[1].mFormat = DataFormat::Vec3;
    attributes[1].mRelativeOffset = offsetof(Vertex, normal);
    attributes[1].mShaderLocation = LUM_LAYOUT_NORMAL;

    attributes[2].mFormat = DataFormat::Vec2;
    attributes[2].mRelativeOffset = offsetof(Vertex, uv);
    attributes[2].mShaderLocation = LUM_LAYOUT_UV;

    VertexLayoutDescriptor cubeVAODesc;
    cubeVAODesc.mStride = sizeof(Vertex);
    cubeVAODesc.mAttributes = attributes;
    return device->CreateVertexLayout(cubeVAODesc, vbo);
}
auto CreateCubeVBO() {
    BufferDescriptor cubeVBODesc;
    cubeVBODesc.mBufferUsage = BufferUsage::Dynamic;
    cubeVBODesc.mData = cubeVerts.data();
    cubeVBODesc.mMapFlags = Mapflag::Write;
    cubeVBODesc.mSize = ByteSize(cubeVerts);
    return device->CreateVertexBuffer(cubeVBODesc);
}
auto CreateCubeTexture() {
    TextureDescriptor textureDesc;
    textureDesc.mFilename = "textures/scene.png";
    textureDesc.bGenerateMipmaps = true;
    textureDesc.mMipmapLevels = 0;
    auto texture = device->CreateTexture2D(textureDesc);
    return texture;
}
auto CreateCubePipeline(auto shader) {
    PipelineDescriptor cubePipeline;
    //cubePipeline.mBlend.bEnabled = true;
    cubePipeline.mBlend.mDstAlphaFactor = BlendFactor::SrcAlpha;
    cubePipeline.mBlend.mSrcAlphaFactor = BlendFactor::SrcAlpha;
    cubePipeline.mBlend.mDstColorFactor = BlendFactor::SrcAlpha;
    cubePipeline.mBlend.mSrcColorFactor = BlendFactor::SrcAlpha;
    cubePipeline.mBlend.mAlphaOp = BlendOp::Max;
    cubePipeline.mBlend.mColorOp = BlendOp::Max;

    cubePipeline.mCull.bEnabled = true;
    cubePipeline.mCull.mFace = Face::Back;

    cubePipeline.mDepthStencil.depth.bEnabled = true;
    cubePipeline.mDepthStencil.depth.bWriteToZBuffer = false;
    cubePipeline.mDepthStencil.depth.mCompare = CompareFlag::Less;

    //cubePipeline.rasterizer.topologyMode = TopologyMode::Line;

    //cubePipeline.cull.bEnabled = true;

    cubePipeline.mShader = shader;
    return device->CreatePipeline(cubePipeline);
}

auto CreateSkyboxEBO() {
    BufferDescriptor cubeEBODesc;
    cubeEBODesc.mBufferUsage = BufferUsage::Static;
    cubeEBODesc.mData = skyboxIndices.data();
    cubeEBODesc.mSize = ByteSize(skyboxIndices);
    return device->CreateElementBuffer(cubeEBODesc);
}
auto CreateSkyboxVAO(auto vbo) {
    VertexLayoutDescriptor vaoDesc;
    std::vector<VertexAttribute> attr(1);
    attr[0].mFormat = DataFormat::Vec3;
    attr[0].mRelativeOffset = offsetof(Vertex, position);
    attr[0].mShaderLocation = LUM_LAYOUT_POSITION;

    vaoDesc.mAttributes = attr;
    vaoDesc.mStride = sizeof(Vertex);
    return device->CreateVertexLayout(vaoDesc, vbo);
}
auto CreateSkyboxVBO() {
    BufferDescriptor vboDesc;
    vboDesc.mBufferUsage = BufferUsage::Static;
    vboDesc.mData = skyboxVerts.data();
    vboDesc.mSize = ByteSize(skyboxVerts);
    return device->CreateVertexBuffer(vboDesc);
}
auto CreateSkyboxTexture() {
    TextureCubemapDescriptor cubemapTexDesc;
    cubemapTexDesc.mFaces[LUM_CUBEMAP_NEGATIVE_X] = "textures/left.png";
    cubemapTexDesc.mFaces[LUM_CUBEMAP_NEGATIVE_Y] = "textures/right.png";
    cubemapTexDesc.mFaces[LUM_CUBEMAP_NEGATIVE_Z] = "textures/top.png";
    cubemapTexDesc.mFaces[LUM_CUBEMAP_POSITIVE_X] = "textures/bottom.png";
    cubemapTexDesc.mFaces[LUM_CUBEMAP_POSITIVE_Y] = "textures/front.png";
    cubemapTexDesc.mFaces[LUM_CUBEMAP_POSITIVE_Z] = "textures/back.png";
    return device->CreateCubemapTexture(cubemapTexDesc);
}
auto CreateSkyboxPipeline(auto shader) {
    PipelineDescriptor skyboxPipeline;
    skyboxPipeline.mDepthStencil.depth.bEnabled = true;
    skyboxPipeline.mDepthStencil.depth.bWriteToZBuffer = false;
    skyboxPipeline.mDepthStencil.depth.mCompare = CompareFlag::LessEqual;

    skyboxPipeline.mCull.bEnabled = true;

    skyboxPipeline.mShader = shader;
    return device->CreatePipeline(skyboxPipeline);
}

float32 scrollX = 0.0, scrollY = 0.0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    scrollX += xoffset;
    scrollY += yoffset;
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
    
    Logger::Get().EnableLog(LogSeverity::All);

    WindowDescriptor windowDesc;
    windowDesc.bFullscreen = false;
    windowDesc.mHeight = 920;
    windowDesc.mWidth = 1280;

    Window* window = CreateWindow(windowDesc);
    device = CreateDevice(window);
    input::SetActiveWindow(static_cast<GLFWwindow*>(window->GetNativeWindow()));

    auto basicShader = CreateShader("geometry_pass.vert", "geometry_pass.frag");
    auto skyboxShader = CreateShader("skybox_pass.vert", "skybox_pass.frag");
    
    AssetService::SetProjectRoot("C:/Users/szymek/Desktop/lumen_assets");
    bool s;
    ModelData model = AssetService::LoadMesh("models/scene.gltf", s);
    cubeIndices = model.mIndices;
    cubeVerts = model.mVertices;

    BaseObject cube;
    BaseObject skybox;

    cube.VBO        = CreateCubeVBO();
    cube.VAO        = CreateCubeVAO(cube.VBO);
    cube.EBO        = CreateCubeEBO();
    cube.texture    = CreateCubeTexture();
    device->AttachElementBufferToLayout(cube.EBO, cube.VAO);

    skybox.VBO = CreateSkyboxVBO();
    skybox.VAO = CreateSkyboxVAO(skybox.VBO);
    skybox.EBO = CreateSkyboxEBO();
    skybox.texture = CreateSkyboxTexture();
    device->AttachElementBufferToLayout(skybox.EBO, skybox.VAO);

    Camera c{ window };
    CameraUBO camUBO{};

    BufferDescriptor uboDesc;
    uboDesc.mBufferUsage = BufferUsage::Dynamic;
    uboDesc.mMapFlags = Mapflag::Write;
    uboDesc.mSize = sizeof(CameraUBO);
    auto cameraUBO = device->CreateUniformBuffer(uboDesc);

    BufferDescriptor muboDesc;
    muboDesc.mBufferUsage = BufferUsage::Dynamic;
    muboDesc.mMapFlags = Mapflag::Write;
    muboDesc.mSize = sizeof(math::Mat4);
    auto modelUBO = device->CreateUniformBuffer(muboDesc);
    
    device->SetUniformBufferBinding(cameraUBO, LUM_UBO_CAMERA_BINDING);
    device->SetUniformBufferBinding(modelUBO, LUM_UBO_MODEL_BINDING);

    SamplerDescriptor sdesc;
    sdesc.mMagFilter = SamplerMagFilter::Linear;
    sdesc.mMinFilter = SamplerMinFilter::LinearMipmapLinear;
    
    sdesc.mAnisotropy = 16;
    sdesc.mWrapS = SamplerWrap::Repeat;
    sdesc.mWrapT = SamplerWrap::Repeat;

    auto sampler = device->CreateSampler(sdesc);

    auto cubePip = CreateCubePipeline(basicShader);
    auto skyboxPip = CreateSkyboxPipeline(skyboxShader);

    TextureDescriptor newdesc;
    newdesc.bGenerateMipmaps = true;
    newdesc.mFilename = "textures/default.png";

    bool success;
    auto data = AssetService::LoadTexture(newdesc.mFilename, success);
    
    glfwSetScrollCallback(static_cast<GLFWwindow*>(window->GetNativeWindow()), scroll_callback);

    do {

        UpdateCamera(device, c, cameraUBO, camUBO, modelUBO, cube);
        c.Update();

        int32 x, y;
        glfwGetWindowSize(static_cast<GLFWwindow*>(window->GetNativeWindow()), &x, &y);
        window->SetWidth(x);
        window->SetHeight(y);

        device->BeginFrame();
        
        c.move_speed = std::clamp((scrollY * 0.005f), 0.0f, 100.f);

        if (input::KeyPressedOnce(input::Key::SPACE)) {
            device->UpdateTexture(cube.texture, newdesc);
        }

        device->BindPipeline(cubePip);
        device->BindSampler(sampler, LUM_SAMPLER_ALBEDO);
        cube.Draw(LUM_TEX_ALBEDO, cubeIndices.size());

        device->BindPipeline(skyboxPip);
        device->BindSampler(sampler, LUM_SAMPLER_CUBEMAP);
        skybox.Draw(LUM_TEX_CUBEMAP, skyboxIndices.size());
        
        device->EndFrame();

    } while (window->IsOpen());

    device->GetProfilerInfo();

}