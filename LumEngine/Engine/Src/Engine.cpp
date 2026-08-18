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

		mAudio.Initialize( mEventBus );
		mPlatform.Initialize( info, mEventBus );
		mRes.Initialize( mPlatform, mEventBus );

		{
			RenderModuleCreateInfo desc{};
			desc.mEventBus = &mEventBus;
			desc.mRenderDev = mPlatform.mRenderDevice;
			desc.mResourceModule = &mRes;
			mRender.Initialize( desc );
		}

		mScene.Initialize( mRes, mRender, mAudio, mEventBus );

	}
	void Engine::BeginFrame( ) {

		mPlatform.mWindow.Update( );
		mEventBus.FlushEvents( );
		mRender.mRenderer.BeginFrame( );

	}
	void Engine::EndFrame( ) {

		mRender.mRenderer.EndFrame( );

	}
	void Engine::Tick( ) {

		Scene* scene = mScene.mSceneMgr.GetCurrentScene( );

		if (scene) {
			TransformSystem::Update( *scene );
			mAudio.mAudioMgr.UpdateInstances( &scene->mEntityMgr );
			mRender.mRenderSys.Update( *scene, &mPlatform.mWindow );
		}

		mAudio.mAudioDevice->SubmitFrame( );

	}
	void Engine::Finalize( ) {

		mPlatform.Finalize( );

	}

	float64 Engine::GetDeltaTime( ) {

		static float64 sLastTime = 0;
		float64 currentTime = mPlatform.mWindow.GetTime( );
		float64 delta = currentTime - sLastTime;
		sLastTime = mPlatform.mWindow.GetTime( );
		return delta;

	}

}