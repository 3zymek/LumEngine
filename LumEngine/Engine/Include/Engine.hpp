/*******************************************************/
/* /                                                 \ */
/*|-------------------|LumEngine|---------------------|*/
/*|            Cross-Platform Game Engine             |*/
/*|===================================================|*/
/*|                    GitHub:                        |*/
/*|       https://github.com/3zymek/LumEngine         |*/
/*|                                                   |*/
/*|        Copyright (c) 2025-present 3zymek          |*/
/*|          Licensed under the MIT License           |*/
/* \                                                 / */
/*******************************************************/
#pragma once

#include <chrono>

#include "AudioModule.hpp"
#include "CreateInfo.hpp"
#include "PlatformModule.hpp"
#include "RenderModule.hpp"
#include "SceneModule.hpp"

#include "Core/CoreDefines.hpp"
#include "Core/CoreTypes.hpp"
#include "Event/EventBus.hpp"
#include "Platform/FileSystem/Path.hpp"

namespace lum {

	class LUM_API Engine {
	public:

		void Initialize( EngineCreateInfo& info );
		void SetScene( const Path& path ) { m_SceneModule.m_SceneMgr.SetScene( path ); }
		void BeginFrame( );
		void EndFrame( );
		void Tick( );
		void Finalize( );
		bool Running( ) { return m_Platform.m_Window.Value( ).IsOpen( ); }
		float64 GetDeltaTime( ) const noexcept;

		ev::EventBus& EventBus( ) { return m_EventBus; }
		AudioModule& Audio( ) { return m_AudioModule; }
		PlatformModule& Platform( ) { return m_Platform; }
		RenderModule& Render( ) { return m_RenderModule; }
		SceneModule& Scene( ) { return m_SceneModule; }

	private:

		using Clock = std::chrono::steady_clock;

		Clock::time_point m_FrameStart{};
		Clock::time_point m_LastDelta{};

		float64 m_DeltaTime = 0.0;
		uint32 m_MaxFps = 120;

		// Initialization order
		ev::EventBus	m_EventBus{};
		AudioModule		m_AudioModule{};
		PlatformModule	m_Platform{};
		RenderModule	m_RenderModule{};
		SceneModule		m_SceneModule{};

	};

} // namespace lum