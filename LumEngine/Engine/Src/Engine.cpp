//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core engine class — owns and orchestrates all engine modules.
//
//=============================================================================//

#include "Engine.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void Engine::Initialize( const EngineCreateInfo& info ) {

		ResourceLoader::SetProjectRoot( info.mProjectDir );

		mAudioModule.Initialize( mEventBus );
		mPlatform.Initialize( info, mEventBus );
		mResourceModule.Initialize( mPlatform, mEventBus );

		{
			RenderModuleCreateInfo desc{};
			desc.mEventBus = mEventBus;
			desc.mRenderDev = mPlatform.mRenderDevice.get();
			desc.mResourceModule = &mResourceModule;
			mRenderModule.Initialize( desc );
		}

		{
			SceneModuleCreateInfo desc{};
			desc.mAudioModule = mAudioModule;
			desc.mResourceModule = mResourceModule;
			desc.mRenderModule = mRenderModule;
			desc.mEventBus = mEventBus;
			mSceneModule.Initialize( desc );
		}

	}
	void Engine::BeginFrame( ) {

		if(mPlatform.mWindow.HasValue())
			mPlatform.mWindow.Value().Update( );
		mEventBus.FlushEvents( );
		mRenderModule.mRenderer.BeginFrame( );

	}
	void Engine::EndFrame( ) {

		mRenderModule.mRenderer.EndFrame( );

	}
	void Engine::Tick( ) {

		Scene* scene = mSceneModule.mSceneMgr.GetCurrentScene( );

		if (scene) {
			TransformSystem::Update( *scene );
			mAudioModule.mAudioMgr.UpdateInstances( &scene->mEntityMgr );
			mRenderModule.mRenderSys.Update( *scene );
		}

		mAudioModule.mAudioDevice->SubmitFrame( );

	}
	void Engine::Finalize( ) {

		mPlatform.Finalize( );

	}

	float64 Engine::GetDeltaTime( ) {
		using clock = std::chrono::high_resolution_clock;

		static auto sLastTime = clock::now( );
		auto currentTime = clock::now( );

		std::chrono::duration<float64> delta = currentTime - sLastTime;
		sLastTime = currentTime;

		return delta.count( );
	}

}