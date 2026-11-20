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

#include "Scene/SceneManager.hpp"
#include "Core/CoreDefines.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Entity/TransformSystem.hpp"

namespace lum {

	class RenderModule;
	class AudioModule;
	namespace ev { class EventBus; }

	struct LUM_API SceneModuleCreateInfo {

		SafePtr<RenderModule>	m_RenderModule = nullptr;
		SafePtr<AudioModule>	m_AudioModule = nullptr;
		SafePtr<ev::EventBus>	m_EventBus = nullptr;

	};

	// Owns the scene manager and wires it up with all engine subsystems.
	// Must be initialized last — depends on platform, resources, render and audio.
	class LUM_API SceneModule {
	public:

		SceneManager m_SceneMgr{};

		void Initialize(const SceneModuleCreateInfo& info);

	};

} // namespace lum