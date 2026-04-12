//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core engine class — owns and orchestrates all engine modules.
//
//=============================================================================//
#pragma once

#include "core/utils/logger.hpp"

#include "event/event_bus.hpp"

#include "platform_module.hpp"
#include "audio_module.hpp"
#include "render_module.hpp"
#include "res_module.hpp"
#include "scene_module.hpp"

namespace lum {

	class Engine {
	public:

		Engine( ) = default;

		void Initialize( StringView projectDir );
		void SetScene( StringView path ) { mScene.mSceneMgr.SetScene( path ); }
		void BeginFrame( );
		void EndFrame( );
		void Tick( );
		void Finalize( );

		ev::EventBus&		GetEventBus( ) { return mEvBus; }
		MAudioModule&		GetAudio( ) { return mAudio; }
		MPlatformModule&	GetPlatform( ) { return mPlatform; }
		MResourceModule&	GetResource( ) { return mRes; }
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