//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core engine class — owns and orchestrates all engine modules.
//
//=============================================================================//

#include "engine.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void Engine::Initialize( StringView projectDir ) {
	    
		AssetLoader::SetProjectRoot( projectDir );

		mAudio.Initialize( mEvBus );
		mPlatform.Initialize( mEvBus );
		mRes.Initialize( mPlatform, mEvBus );
		mRender.Initialize( mPlatform,  mRes, mEvBus );
		mScene.Initialize( mRes, mRender, mAudio, mEvBus );

	}
	void Engine::BeginFrame( ) {

		mPlatform.mWindow.Update( );

		mEvBus.PollEvents( );

		mRender.mRenderer.BeginFrame( );
		
	}
	void Engine::EndFrame( ) {
		
		mRender.mRenderer.EndFrame( );

	}
	void Engine::Tick( ) {

		mRender.mRenderSys.Update( &mScene.mSceneMgr.GetCurrentScene( )->mEntityMgr, &mPlatform.mWindow );

		mAudio.mAudioMgr.Update( &mScene.mSceneMgr.GetCurrentScene( )->mEntityMgr );
		mAudio.mAudioDevice->EndFrame( );

	}
	void Engine::Finalize( ) {

		mPlatform.Finalize( );

	}

}