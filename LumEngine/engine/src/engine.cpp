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
		mRes.Initialize( mPlatform );
		mRender.Initialize( mPlatform, mRes, mEvBus );
		mScene.Initialize( mRes, mRender, mAudio, mEvBus );

	}
	void Engine::SetScene( StringView path ) {

		mScene.mSceneMgr.SetScene( path );

	}
	void Engine::Run( ) {

		mEvBus.SubscribePermanently<EKeyPressed>( [&]( const EKeyPressed& ev ) {

			if (ev.mKey == input::Key::SPACE)
				SetScene( "scene2.lsc" );


		} );

		while (mPlatform.mWindow.IsOpen( )) {

			mEvBus.PollEvents( );

			mPlatform.mWindow.Update( );
			mRender.mRenderer.BeginFrame( );

			ImGui::Begin( "Scene" );

			ImGui::Text( "Directional Light" );

			render::FDirectionalLight light = mRender.mRenderer.GetDirectionalLight( );

			bool changed = false;
			changed |= ImGui::DragFloat3( "Direction", &light.mDirection.x, 0.01f, -1.0f, 1.0f );
			changed |= ImGui::ColorEdit3( "Color", &light.mColor.x );
			changed |= ImGui::DragFloat( "Intensity", &light.mIntensity, 0.01f, 0.0f, 10.0f );

			if (changed) {
				light.mDirection = glm::normalize( light.mDirection );
				mRender.mRenderer.SetDirectionalLight( light );
			}

			ImGui::End( );

			mRender.mRenderSys.Update( &mScene.mSceneMgr.GetCurrentScene( )->mEntityMgr, &mPlatform.mWindow );

			mAudio.mAudioMgr.Update( &mScene.mSceneMgr.GetCurrentScene( )->mEntityMgr );
			mAudio.mAudioDevice->EndFrame( );

			mRender.mRenderer.EndFrame( );
		}

	}
	void Engine::Finalize( ) {

		mPlatform.Finalize( );

	}

}