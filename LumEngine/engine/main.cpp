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
#include "core/utils/asset_loader.hpp"
#include "rhi/rhi_common.hpp"
#include "core/shaders_define.h"
#include "core/math/backend/gtx/string_cast.hpp"
#include "imgui.h"
#include "core/utils/flags.hpp"
#include "core/utils/fixed_string.hpp"
#include "assimp/Importer.hpp"

#include "testfield/renderer.hpp"

using namespace lum;
using namespace lum::rhi;

int main() {

    Logger::Get().EnableLog(LogSeverity::All);

    WindowDescriptor windowDesc;
    windowDesc.bFullscreen = false;
    windowDesc.mHeight = 920;
    windowDesc.mWidth = 1280;

    Window* window = CreateWindow(windowDesc);

    input::SetActiveWindow(static_cast<GLFWwindow*>(window->GetNativeWindow()));

    AssetLoader::SetProjectRoot("C:/Users/szymek/Desktop/lumen_assets");

    Renderer render{ window };

    Camera camera{ window };

    render.mLights[0];

    auto texture = render.LoadTexture2D("textures/cola_baseColor.png", TexturePreset::Albedo);

    MaterialDescriptor desc;
    desc.mAlbedoMap = render.GetTexture(texture);
    auto handle = render.mMaterialManager.CreateMaterial(desc);
    auto def = render.mMaterialManager.Get(handle);

    Object sphere;
    render.LoadModel(sphere, "models/sphere.fbx");
    sphere.mMaterial = def;

    Object bottle;
    render.LoadModel(bottle, "models/bottle/scene.gltf");

    /*
    Material& material = renderer.CreateMaterial();
    material.options = default; // example
    
    
    */

    bottle.mTransform.position = { 10.f, 0.f, 0.f };

    while (window->IsOpen()) {

        render.BeginFrame();

        ImGui::Begin("Lights");

        for (int32 i = 0; i < LUM_MAX_LIGHTS; i++) {
            String label = "Light " + std::to_string(i);

            
            if (ImGui::CollapsingHeader(label.c_str())) {
                ImGui::PushID(i);

                ImGui::DragFloat3("Position", glm::value_ptr(render.mLights[i].mPosition), 0.1f, -100.f, 100.f);
                ImGui::ColorEdit3("Color", glm::value_ptr(render.mLights[i].mColor));
                ImGui::DragFloat("Intensity", &render.mLights[i].mIntensity, 0.1f, 0.0f, 10.0f);

                ImGui::Spacing();

                ImGui::PopID();
            }
        }

        ImGui::ColorEdit3("Sphere color", glm::value_ptr(sphere.mMaterial.mBaseColor));

        ImGui::End();

        render.UpdateCamera(camera);

        render.Draw(sphere);
        //render.Draw(bottle);

        render.EndFrame();

    }

}