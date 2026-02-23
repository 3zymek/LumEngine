#pragma once
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
#include "core/utils/flags.hpp"
#include "core/utils/fixed_string.hpp"
#include "render/texture_manager.hpp"
#include "render/mesh_manager.hpp"
#include "render/shader_manager.hpp"
#include "render/material_manager.hpp"
#include "render/renderer.hpp"
#include "render/render_system.hpp"

#include "scene/scene_manager.hpp"

namespace lum {

	struct AudioModule {

		

	};

	struct PlatformModule {

		Window* mWindow = nullptr;
		rhi::RDevice* mRenderDevice = nullptr;

		void Initialize( ) {

			mWindow = CreateWindow({});
			input::SetActiveWindow(static_cast<GLFWwindow*>(mWindow->GetNativeWindow()));

			mRenderDevice = rhi::CreateDevice(mWindow);

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

		void Initialize( PlatformModule& platform, ResourceModule& res ) {
			render::FRendererContext ctx;
			ctx.mMaterialManager = &res.mMaterialMgr;
			ctx.mMeshManager = &res.mMeshMgr;
			ctx.mTextureManager = &res.mTextureMgr;
			ctx.mRenderDevice = platform.mRenderDevice;
			ctx.mShaderManager = &res.mShaderMgr;

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

		void Initialize( ccharptr projectDir ) {
			
			AssetLoader::SetProjectRoot(projectDir);

			init();

		}
		void SetScene( ccharptr path ) {

			mScene.mSceneMgr.SetScene(path);
		
		}
		void Run( ) {

			while (mPlatform.mWindow->IsOpen()) {

				mPlatform.mWindow->PollEvents();

				mRender.mRenderer.BeginFrame();

				mRender.mRenderSys.Update( mScene.mSceneMgr.GetCurrentScene()->mEntityMgr.get(), mPlatform.mWindow );

				mRender.mRenderer.EndFrame();

			}

		}
		void Finalize( ) {

			finalize();

		}

	private:

		void init() {

			mPlatform.	Initialize();
			mRes.		Initialize(mPlatform);
			mRender.	Initialize(mPlatform, mRes);
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