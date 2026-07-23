//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core engine class — owns and orchestrates all engine modules.
//
//=============================================================================//
#pragma once

#include "Core/Utils/Logger.hpp"

#include "Event/EventBus.hpp"

#include "PlatformModule.hpp"
#include "AudioModule.hpp"
#include "RenderModule.hpp"
#include "ResModule.hpp"
#include "SceneModule.hpp"

namespace lum {
	
	class LUM_API Engine {
	public:

		Engine( ) = default;

		void Initialize( StringView projectDir );
		void SetScene( StringView path ) { mScene.mSceneMgr.SetScene( path ); }
		void BeginFrame( );
		void EndFrame( );
		void Tick( );
		void Finalize( );

		ev::EventBus&		GetEventBus( ) { return mEvBus; }
		MAudioModule&		GetModuleAudio( ) { return mAudio; }
		MPlatformModule&	GetModulePlatform( ) { return mPlatform; }
		MResourceModule&	GetModuleResource( ) { return mRes; }
		MRenderModule&		GetModuleRender( ) { return mRender; }
		MSceneModule&		GeModuleScene( ) { return mScene; }

		float64 GetDeltaTime( ) { 
			
			static float64 sLastTime = 0;
			float64 currentTime = mPlatform.mWindow.GetTime( );
			float64 delta = currentTime - sLastTime;
			sLastTime = mPlatform.mWindow.GetTime( );
			return delta;

		}

		bool IsRunning( ) { return mPlatform.mWindow.IsOpen( ); }

	private:

		// Initialization order
		ev::EventBus	mEvBus;
		MAudioModule	mAudio;
		MPlatformModule	mPlatform;
		MResourceModule	mRes;
		MRenderModule	mRender;
		MSceneModule	mScene;

	};

} // namespace lum