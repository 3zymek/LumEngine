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
		void SetScene( const Path& path ) { m_SceneModule.m_SceneMgr.SetScene( path ); }
		void BeginFrame( );
		void EndFrame( );
		void Tick( );
		void Finalize( );
		float64 GetDeltaTime( );

		ev::EventBus&		EventBus( ) { return m_EventBus; }
		AudioModule&		Audio( ) { return m_AudioModule; }
		PlatformModule&		Platform( ) { return m_Platform; }
		ResourceModule&		Resource( ) { return m_ResourceModule; }
		RenderModule&		Render( ) { return m_RenderModule; }
		SceneModule&		Scene( ) { return m_SceneModule; }

	private:

		// Initialization order
		ev::EventBus	m_EventBus;
		AudioModule		m_AudioModule;
		PlatformModule	m_Platform;
		ResourceModule	m_ResourceModule;
		RenderModule	m_RenderModule;
		SceneModule		m_SceneModule;

	};

} // namespace lum