#include "core/core_pch.hpp"
#include "lum_packages/lum_audio.hpp"
#include "lum_packages/lum_ecs.hpp"
#include "lum_packages/lum_events.hpp"
#include "lum_packages/lum_render.hpp"
#include "platform/input_common.hpp"
#include "core/utils/logger.hpp"
#include "audio/components/c_audio_listener.hpp"
#include "platform/window.hpp"
#include "rhi/core/rhi_device.hpp"
#include "core/utils/asset_loader.hpp"
#include "rhi/rhi_common.hpp"
#include "core/shaders_define.h"
#include "core/math/backend/gtx/string_cast.hpp"
#include "imgui.h"
#include "core/utils/flags.hpp"
#include "core/utils/fixed_string.hpp"
#include "render/renderer.hpp"
#include "render/texture_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/render_system.hpp"
#include "render/camera.hpp"

#include "engine.hpp"

using namespace lum;
using namespace lum::rhi;

int main() {

    Engine e;

    e.Initialize("C:/Users/szymek/Desktop/lumen_assets");

    e.SetScene("scene.json");
    
    e.Run();

    e.Finalize();



    /*
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
    auto cubemap = texManager.LoadEquirectangularCubemap("textures/cubemap.png");

    FMaterialBase base;
    base.mBaseColor = { 1.f, 1.f, 1.f };
    auto baseHandle = matManager.UploadBase(base);

    auto materialInstance = matManager.CreateInstance(baseHandle);

    auto meshHandle = meshManager.CreateStatic("models/Can.obj");

    render::FRendererContext ctx;
    ctx.mRenderDevice = device;
    ctx.mTextureManager = &texManager;
    ctx.mMaterialManager = &matManager;
    ctx.mMeshManager = &meshManager;
    ctx.mShaderManager = &shaderManager;

    render::Renderer render;
    render.Initialize(ctx);
    ecs::MEntityManager entMgr;
    RenderSystem sysRender{ &entMgr, &render };

    EditorCamera camera{ window };
    ManagedEntity e1 = entMgr.CreateEntity();

    e1.AddComponent<CMaterial>({ .mMaterial = materialInstance });
    e1.AddComponent<CTransform>();
    e1.AddComponent<CStaticMesh>({ .mMesh = meshHandle });
    //e1.AddComponent<CRender>();

    render.SetEnvionmentTexture(cubemap);

    render::DirectionalLight light;

    render.mDirectionalLight = &light;

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

        //ImGui::Begin("BaseColor");
        //ImGui::DragFloat("Roughness1", &obj.mMaterial.mRoughness, 0.1f, 0.0f, 1.0f);
        //ImGui::DragFloat("Metallic1", &obj.mMaterial.mMetallic, 0.1f, 0.0f, 1.0f);
        //ImGui::ColorEdit3("Color1", glm::value_ptr(obj.mMaterial.mBaseColor));
        //ImGui::DragFloat("Roughness2", &obj2.mMaterial.mRoughness, 0.1f, 0.0f, 1.0f);
        //ImGui::DragFloat("Metallic2", &obj2.mMaterial.mMetallic, 0.1f, 0.0f, 1.0f);
        //ImGui::ColorEdit3("Color2", glm::value_ptr(obj2.mMaterial.mBaseColor));
        //ImGui::End();

        ImGui::Begin("Light");
        ImGui::DragFloat3("Position", glm::value_ptr(light.mDirection), 0.1f, -1000.f, 1000.f);
        ImGui::DragFloat("Intensity", &light.mIntensity, 0.1f, 0.0f, 1000.f);
        ImGui::End();

        render.UpdateCamera(camera);

        sysRender.Update();

        ImGui::Begin("Spawn");
        if (ImGui::Button("Render component")) {
            if (!e1.HasComponent<CRender>())
                e1.AddComponent<CRender>();
            else
                e1.RemoveComponent<CRender>();
        }
        ImGui::End();

        render.EndFrame();
    }
    */
} 