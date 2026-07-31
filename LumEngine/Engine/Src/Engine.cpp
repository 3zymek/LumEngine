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

	void Engine::Initialize( EngineCreateInfo info ) {
	    
		

		ResourceLoader::SetProjectRoot( info.mProjectDirectory );

		mAudio.Initialize( mEventBus );
		mPlatform.Initialize( info, mEventBus );
		mRes.Initialize( mPlatform, mEventBus );
		mRender.Initialize( mPlatform,  mRes, mEventBus );
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

		TransformSystem::Update( mScene.mSceneMgr.GetCurrentScene( ) );

		mRender.mRenderSys.Update( mScene.mSceneMgr.GetCurrentScene( ), &mPlatform.mWindow );

		mAudio.mAudioMgr.UpdateInstances( &mScene.mSceneMgr.GetCurrentScene( )->mEntityMgr );
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