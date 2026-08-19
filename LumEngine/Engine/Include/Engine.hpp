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

		void Initialize( const EngineCreateInfo& info );
		void SetScene( const Path& path ) { mSceneModule.mSceneMgr.SetScene( path ); }
		void BeginFrame( );
		void EndFrame( );
		void Tick( );
		void Finalize( );
		float64 GetDeltaTime( );

		ev::EventBus&		GetEventBus( ) { return mEventBus; }
		AudioModule&		GetModuleAudio( ) { return mAudioModule; }
		PlatformModule&		GetModulePlatform( ) { return mPlatform; }
		ResourceModule&		GetModuleResource( ) { return mResourceModule; }
		RenderModule&		GetModuleRender( ) { return mRenderModule; }
		SceneModule&		GetModuleScene( ) { return mSceneModule; }

	private:

		// Initialization order
		ev::EventBus	mEventBus;
		AudioModule		mAudioModule;
		PlatformModule	mPlatform;
		ResourceModule	mResourceModule;
		RenderModule	mRenderModule;
		SceneModule		mSceneModule;

	};

} // namespace lum