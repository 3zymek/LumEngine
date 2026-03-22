//========= Copyright (C) 2026 3zymek, MIT License ============//
//
// Purpose: Core engine class — owns and orchestrates all engine modules.
//
//=============================================================================//
#pragma once

#include "core/utils/logger.hpp"
#include "core/utils/asset_loader.hpp"

#include "event/event_bus.hpp"

#include "platform_module.hpp"
#include "audio_module.hpp"
#include "render_module.hpp"
#include "res_module.hpp"
#include "scene_module.hpp"

#include "imgui.h"

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

		float64 GetDeltaTime( ) const noexcept { return mPlatform.mWindow.GetDeltaTime( ); }

		ev::EventBus& GetEventBus( ) { return mEvBus; }
		MAudioModule& GetAudio( ) { return mAudio; }
		MPlatformModule& GetPlatform( ) { return mPlatform; }
		MRenderModule& GetRender( ) { return mRender; }
		MSceneManager& GetScene( ) { return mScene.mSceneMgr; }


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