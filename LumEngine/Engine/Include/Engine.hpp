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

		void Initialize( StringView projectDir );
		void SetScene( StringView path ) { mScene.mSceneMgr.SetScene( path ); }
		void BeginFrame( );
		void EndFrame( );
		void Tick( );
		void Finalize( );

		float64 GetDeltaTime( );
		bool IsRunning( ) { return mPlatform.mWindow.IsOpen( ); }

		ev::EventBus&		GetEventBus( ) { return mEvBus; }
		AudioModule&		GetModuleAudio( ) { return mAudio; }
		PlatformModule&		GetModulePlatform( ) { return mPlatform; }
		ResourceModule&		GetModuleResource( ) { return mRes; }
		RenderModule&		GetModuleRender( ) { return mRender; }
		SceneModule&		GeModuleScene( ) { return mScene; }

	private:

		// Initialization order
		ev::EventBus	mEvBus;
		AudioModule		mAudio;
		PlatformModule	mPlatform;
		ResourceModule	mRes;
		RenderModule	mRender;
		SceneModule	mScene;

	};

} // namespace lum