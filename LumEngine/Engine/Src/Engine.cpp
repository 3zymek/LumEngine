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

#include "Core/CorePch.hpp"

#include "Core/Utils/SafePtr.hpp"
#include "Core/CoreTypes.hpp"
#include "Core/Utils/ResourceLoader.hpp"
#include "CreateInfo.hpp"
#include "Engine.hpp"

namespace lum {

	//=======================================================//
	// Public
	//=======================================================//

	void Engine::Initialize(EngineCreateInfo& info) {

		ResourceLoader::SetProjectRoot(info.m_ProjectDir);

		m_AudioModule.Initialize(m_EventBus);
		m_Platform.Initialize(info, m_EventBus);

		{
			RenderModuleCreateInfo desc{};
			desc.m_EventBus = m_EventBus;
			desc.m_RenderDevice = m_Platform.m_RenderDevice();
			m_RenderModule.Initialize(desc);
		}

		{
			SceneModuleCreateInfo desc{};
			m_SceneModule.Initialize(desc);
		}

	}
	void Engine::BeginFrame() {

		if (m_Platform.m_Window.HasValue())
			m_Platform.m_Window.Value().Update();

		m_EventBus.FlushEvents();

		m_RenderModule.m_Renderer.BeginFrame();

	}
	void Engine::EndFrame() {

		m_RenderModule.m_Renderer.EndFrame();

	}
	void Engine::Tick() {

		auto scene = m_SceneModule.m_SceneMgr.GetCurrentScene();

		if (scene) {
			TransformSystem::Update(scene.Ref());
			m_AudioModule.m_AudioMgr.UpdateInstances(scene().m_EntityMgr);
		}

		m_AudioModule.m_AudioDevice().SubmitFrame();

	}
	void Engine::Finalize() {

		m_Platform.Finalize();

	}

	float64 Engine::GetDeltaTime() {
		using clock = std::chrono::high_resolution_clock;

		static auto s_LastTime = clock::now();
		auto currentTime = clock::now();

		std::chrono::duration<float64> delta = currentTime - s_LastTime;
		s_LastTime = currentTime;

		return delta.count();
	}

} // namespace lum
