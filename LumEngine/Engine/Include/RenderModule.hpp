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

#include "Core/CoreTypes.hpp"
#include "Core/Utils/SafePtr.hpp"
#include "Rhi/Interfaces/Core/RhiDevice.hpp"
#include "Rendering/Renderer.hpp"
//#include "Render/RenderSystem.hpp"

namespace lum {

	class PlatformModule;
	class ResourceModule;
	namespace ev { class EventBus; }

	struct RenderModuleCreateInfo {

		SafePtr<rhi::IRenderDevice> m_RenderDevice = nullptr;
		SafePtr<ResourceModule> m_ResourceModule = nullptr;
		SafePtr<ev::EventBus> m_EventBus = nullptr;
		uint32 m_ViewportWidth = 1280;
		uint32 m_ViewportHeight = 720;

	};

	// Owns the renderer and render system.
	// Must be initialized after PlatformModule and ResourceModule.
	class LUM_API RenderModule {
	public:

		Renderer m_Renderer{};

		void Initialize(const RenderModuleCreateInfo& info);

	};

} // namespace lum