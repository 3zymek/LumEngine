#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "window_context/input_common.hpp"
#include "core/utils/logger.hpp"
#include "audio/components/c_audio_listener.hpp"
#include "window_context/window.hpp"
#include "rhi/core/rhi_device.hpp"
#include "core/utils/asset_loader.hpp"
#include "rhi/rhi_common.hpp"
#include "core/shaders_define.h"
#include "core/math/backend/gtx/string_cast.hpp"
#include "imgui.h"
#include "core/utils/flags.hpp"
#include "core/utils/fixed_string.hpp"
#include "testfield/texture_manager.hpp"
#include "testfield/material_manager.hpp"
#include "testfield/model_manager.hpp"
#include "testfield/shader_manager.hpp"
#include "testfield/renderer.hpp"

using namespace lum;
using namespace lum::rhi;

int main() {

    Logger::Get().EnableLog(LogSeverity::All);

    AssetLoader::SetProjectRoot("C:/Users/szymek/Desktop/lumen_assets");

    WindowDescriptor windowDesc;
    windowDesc.bFullscreen = false;
    windowDesc.mHeight = 920;
    windowDesc.mWidth = 1280;

    Window* window = CreateWindow(windowDesc);

    RDevice* device = rhi::CreateDevice(window);

    input::SetActiveWindow(static_cast<GLFWwindow*>(window->GetNativeWindow()));

    MTextureManager texManager{ device };
    MMaterialManager matManager{ device, &texManager };
    MMeshManager meshManager{ device };
    MShaderManager shaderManager{ device };
    
    auto tex = texManager.Load("textures/scene.png", ETexturePreset::Albedo);

    FMaterialBase base;
    base.mAlbedoMap = tex;
    base.mBaseColor = { 1.f, 0.f, 1.f };
    auto baseHandle = matManager.UploadBase(base);

    auto materialInstance = matManager.CreateInstance(baseHandle);

    auto meshHandle = meshManager.CreateStatic("models/scene.gltf");

    SRenderer render{ device, &texManager, &matManager, &meshManager };

    Camera camera{ window };
    Object obj;
    obj.mMaterial = materialInstance;
    obj.mStaticMesh = meshHandle;

    auto geometry = shaderManager.LoadShader("shaders/geometry_pass.vert", "shaders/geometry_pass.frag", ERootID::Internal);

    while (window->IsOpen()) {
       
        window->PollEvents();
        
        render.BeginFrame();
        
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::Begin(
            "##fps",
            nullptr,
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration
        );
        
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("BaseColor");
        ImGui::ColorEdit3("Color", glm::value_ptr(obj.mMaterial.mBaseColor));
        ImGui::End();

        render.UpdateCamera(camera);
        render.Draw(obj, geometry);

        render.EndFrame();
    }



} 