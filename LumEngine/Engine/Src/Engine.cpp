//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Core engine class — owns and orchestrates all engine modules.
//
//=============================================================================//

#include "Engine.hpp"

namespace lum {
	
	//=======================================================//
	// Public
	//=======================================================//

	void Engine::Initialize( const EngineCreateInfo& info ) {

		ResourceLoader::SetProjectRoot( info.m_ProjectDir );

		m_AudioModule.Initialize( m_EventBus );
		m_Platform.Initialize( info, m_EventBus );
		m_ResourceModule.Initialize( m_Platform, m_EventBus );

		{
			RenderModuleCreateInfo desc{};
			desc.m_EventBus = m_EventBus;
			desc.m_RenderDev = m_Platform.m_RenderDevice.get( );
			desc.m_ResourceModule = &m_ResourceModule;
			m_RenderModule.Initialize( desc );
		}

		{
			SceneModuleCreateInfo desc{};
			desc.m_AudioModule = m_AudioModule;
			desc.m_ResourceModule = m_ResourceModule;
			desc.m_RenderModule = m_RenderModule;
			desc.m_EventBus = m_EventBus;
			m_SceneModule.Initialize( desc );
		}

	}
	void Engine::BeginFrame( ) {

		if (m_Platform.m_Window.HasValue( ))
			m_Platform.m_Window.Value( ).Update( );
		m_EventBus.FlushEvents( );
		m_RenderModule.m_Renderer.BeginFrame( );

	}
	void Engine::EndFrame( ) {

		m_RenderModule.m_Renderer.EndFrame( );

	}
	void Engine::Tick( ) {

		SceneInstance* scene = m_SceneModule.m_SceneMgr.GetCurrentScene( );

		if (scene) {
			TransformSystem::Update( *scene );
			m_AudioModule.m_AudioMgr.UpdateInstances( &scene->m_EntityMgr );
			m_RenderModule.m_RenderSys.Update( *scene );
		}

		m_AudioModule.m_AudioDevice->SubmitFrame( );

	}
	void Engine::Finalize( ) {

		m_Platform.Finalize( );

	}

	float64 Engine::GetDeltaTime( ) {
		using clock = std::chrono::high_resolution_clock;

		static auto sLastTime = clock::now( );
		auto currentTime = clock::now( );

		std::chrono::duration<float64> delta = currentTime - sLastTime;
		sLastTime = currentTime;

		return delta.count( );
	}

} // namespace lum