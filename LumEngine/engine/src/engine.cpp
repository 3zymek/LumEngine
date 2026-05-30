//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core engine class — owns and orchestrates all engine modules.
//
//=============================================================================//

#include "engine.hpp"
#include "core/transform_system.hpp"

namespace lum {

	//---------------------------------------------------------
	// Public
	//---------------------------------------------------------

	void Engine::Initialize( StringView projectDir ) {
	    
		ResourceLoader::SetProjectRoot( projectDir );

		mAudio.Initialize( mEvBus );
		mPlatform.Initialize( mEvBus );
		mRes.Initialize( mPlatform, mEvBus );
		mRender.Initialize( mPlatform,  mRes, mEvBus );
		mScene.Initialize( mRes, mRender, mAudio, mEvBus );

	}
	void Engine::BeginFrame( ) {

		mPlatform.mWindow.Update( );

		mEvBus.FlushEvents( );

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

}