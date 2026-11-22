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

		m_FrameStart = Clock::now( );

		if (m_Platform.m_Window.HasValue())
			m_Platform.m_Window.Value().Update();

		m_EventBus.FlushEvents();

		m_RenderModule.m_Renderer.BeginFrame();

	}
	void Engine::EndFrame() {

		m_RenderModule.m_Renderer.EndFrame();

		if (m_MaxFps <= 0) {

			const auto frameTime = Clock::now( ) - m_FrameStart;
			const auto targetFrameTime = std::chrono::duration<double>( 1.0 / m_MaxFps );

			if (frameTime < targetFrameTime) {
				std::this_thread::sleep_until( m_FrameStart + targetFrameTime );
			}

		}

		m_DeltaTime = std::chrono::duration<float64>( Clock::now( ) - m_LastDelta ).count( );
		m_LastDelta = Clock::now( );

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

	float64 Engine::GetDeltaTime() const noexcept {
		return m_DeltaTime;
	}

} // namespace lum
