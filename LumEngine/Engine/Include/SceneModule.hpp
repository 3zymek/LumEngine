//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Scene module — scene manager lifecycle.
//
//=============================================================================//
#pragma once

#include "Scene/SceneManager.hpp"
#include "Entity/TransformSystem.hpp"

namespace lum {

	class RenderModule;
	class ResourceModule;
	class AudioModule;
	namespace ev { class EventBus; }

	struct LUM_API SceneModuleCreateInfo {

		SafePtr<ResourceModule> m_ResourceModule = nullptr;
		SafePtr<RenderModule>	m_RenderModule = nullptr;
		SafePtr<AudioModule>	m_AudioModule = nullptr;
		SafePtr<ev::EventBus>	m_EventBus = nullptr;

	};

	// Owns the scene manager and wires it up with all engine subsystems.
	// Must be initialized last — depends on platform, resources, render and audio.
	class LUM_API SceneModule {
	public:

		SceneManager m_Scenem_Gr{};

		void Initialize( const SceneModuleCreateInfo& info );

	};

} // namespace lum