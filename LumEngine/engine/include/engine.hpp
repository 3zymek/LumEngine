#pragma once

#include "platform/window.hpp"
#include "platform/input_common.hpp"

#include "core/utils/logger.hpp"
#include "core/utils/asset_loader.hpp"

#include "rhi/core/rhi_device.hpp"

#include "render/texture_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/renderer.hpp"
#include "render/render_system.hpp"
#include "render/common.hpp"

#include "ahi/core/ahi_device.hpp"

#include "audio/audio_manager.hpp"

#include "imgui.h"

#include "event/event_bus.hpp"

#include "scene/scene_manager.hpp"

namespace lum {

	struct AudioModule {

		ahi::AudioDevice* mAudioDevice = nullptr;
		audio::MAudioManager mAudioMgr;

	};

	struct PlatformModule {

		Window mWindow;
		rhi::RenderDevice* mRenderDevice = nullptr;

		void Initialize( ev::EventBus& bus ) {

			WindowDescriptor desc;
			desc.mEventBus = &bus;

			mWindow.Initialize(desc);
			input::SetActiveWindow(static_cast<GLFWwindow*>(mWindow.GetNativeWindow()));

			mRenderDevice = rhi::CreateDevice(rhi::RenderBackend::OpenGL);
			mRenderDevice->Initialize(&mWindow);

		}

		void Finalize( ) {
			mRenderDevice->Finalize();
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

		void Initialize( StringView projectDir ) {
			
			AssetLoader::SetProjectRoot(projectDir);

			init();

		}
		void SetScene( StringView path ) {

			mScene.mSceneMgr.SetScene(path);
		
		}
		void Run( ) {
			
			while (mPlatform.mWindow.IsOpen()) {

				mEvBus.PollEvents();

				mPlatform.mWindow.Update();
				mRender.mRenderer.BeginFrame();

				ImGui::Begin("Scene");

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

				mRender.mRenderSys.Update(&mScene.mSceneMgr.GetCurrentScene()->mEntityMgr, &mPlatform.mWindow);
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