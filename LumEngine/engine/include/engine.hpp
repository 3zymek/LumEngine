#pragma once
#include "core/core_pch.hpp"
#include "platform/input_common.hpp"
#include "core/utils/logger.hpp"
#include "platform/window.hpp"
#include "rhi/core/rhi_device.hpp"
#include "core/utils/asset_loader.hpp"
#include "rhi/rhi_common.hpp"
#include "render/texture_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/renderer.hpp"
#include "render/render_system.hpp"
#include "render/common.hpp"
#include "imgui.h"

#include "scene/scene_manager.hpp"

namespace lum {

	struct AudioModule {

		

	};

	struct PlatformModule {

		Window* mWindow = nullptr;
		rhi::RDevice* mRenderDevice = nullptr;

		void Initialize( ev::EventBus& bus ) {

			WindowDescriptor desc;
			desc.mEventBus = &bus;

			mWindow = CreateWindow(desc);
			if (!mWindow) {
				LUM_LOG_FATAL("Failed to create window");
				return;
			}
			input::SetActiveWindow(static_cast<GLFWwindow*>(mWindow->GetNativeWindow()));

			mRenderDevice = rhi::CreateDevice(mWindow, rhi::RenderBackend::OpenGL);

		}

		void Finalize( ) {
			delete mWindow;
			delete mRenderDevice;
		}

	};

	struct ResourceModule {
		
		MTextureManager		mTextureMgr;
		MMaterialManager	mMaterialMgr;
		MMeshManager		mMeshMgr;
		MShaderManager		mShaderMgr;

		void Initialize( PlatformModule& platform ) {
			
			mTextureMgr.	Initialize(platform.mRenderDevice);
			mMeshMgr.		Initialize(platform.mRenderDevice);
			mShaderMgr.		Initialize(platform.mRenderDevice);
			mMaterialMgr.	Initialize(platform.mRenderDevice, &mTextureMgr);

		}

	};

	struct RenderModule {

		render::Renderer mRenderer;
		render::RenderSystem mRenderSys;

		void Initialize( PlatformModule& platform, ResourceModule& res, ev::EventBus& bus) {

			render::FRendererContext ctx;
			ctx.mMaterialMgr = &res.mMaterialMgr;
			ctx.mMeshMgr = &res.mMeshMgr;
			ctx.mTextureMgr = &res.mTextureMgr;
			ctx.mRenderDevice = platform.mRenderDevice;
			ctx.mShaderMgr = &res.mShaderMgr;
			ctx.mEventBus = &bus;
			mRenderer.Initialize(ctx);

			mRenderSys.Initialize(&mRenderer);
		}

	};

	struct SceneModule {

		MSceneManager mSceneMgr;
		ecs::MEntityManager mEntityMgr;

		void Initialize( ResourceModule& res, RenderModule& render ) {

			FSceneManagerContext ctx;
			ctx.mMaterialMgr = &res.mMaterialMgr;
			ctx.mMeshMgr = &res.mMeshMgr;
			ctx.mTextureMgr = &res.mTextureMgr;
			ctx.mShaderMgr = &res.mShaderMgr;
			ctx.mRenderer = &render.mRenderer;

			mSceneMgr.Initialize(ctx);

		}

	};

	class Engine {
	public:

		Engine( ) = default;

		void Initialize( String projectDir ) {
			
			AssetLoader::SetProjectRoot(projectDir);

			init();

		}
		void SetScene( ccharptr path ) {

			mScene.mSceneMgr.SetScene(path);
		
		}
		void Run( ) {

			while (mPlatform.mWindow->IsOpen()) {
				mPlatform.mWindow->Update();
				mRender.mRenderer.BeginFrame();

				ImGui::Begin("Scene");

				if (ImGui::Button("Reload scene"))
					SetScene("scene.lsc");

				ImGui::Separator();
				ImGui::Text("Directional Light");

				render::FDirectionalLight light = mRender.mRenderer.GetDirectionalLight();

				bool changed = false;
				changed |= ImGui::DragFloat3("Direction", &light.mDirection.x, 0.01f, -1.0f, 1.0f);
				changed |= ImGui::ColorEdit3("Color", &light.mColor.x);
				changed |= ImGui::DragFloat("Intensity", &light.mIntensity, 0.01f, 0.0f, 10.0f);

				if (changed) {
					light.mDirection = glm::normalize(light.mDirection);
					mRender.mRenderer.SetDirectionalLight(light);
				}

				ImGui::End();

				mRender.mRenderSys.Update(&mScene.mSceneMgr.GetCurrentScene()->mEntityMgr, mPlatform.mWindow);
				mRender.mRenderer.EndFrame();
			}

		}
		void Finalize( ) {

			finalize();

		}

	private:

		void init() {

			mPlatform.	Initialize(mEvBus);
			mRes.		Initialize(mPlatform);
			mRender.	Initialize(mPlatform, mRes, mEvBus);
			mScene.		Initialize(mRes, mRender);

		}

		void finalize() {

			mPlatform.Finalize();

		}

		ev::EventBus	mEvBus;
		PlatformModule	mPlatform;
		ResourceModule	mRes;
		RenderModule	mRender;
		SceneModule		mScene;
		
	};

} // namespace lum