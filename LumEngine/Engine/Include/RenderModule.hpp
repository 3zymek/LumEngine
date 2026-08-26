//========= Copyright (C) 2025-present 3zymek, MIT License ============//
//
// Purpose: Render module — renderer and render system lifecycle.
//
//=============================================================================//
#pragma once

#include "Render/Renderer.hpp"
#include "Render/RenderSystem.hpp"

namespace lum {

	class PlatformModule;
	class ResourceModule;
	namespace ev { class EventBus; }

	struct RenderModuleCreateInfo {

		SafePtr<rhi::IRenderDevice> m_RenderDev = nullptr;
		SafePtr<ResourceModule> m_ResourceModule = nullptr;
		SafePtr<ev::EventBus> m_EventBus = nullptr;
		uint32 m_ViewportWidth = 1280;
		uint32 m_ViewportHeight = 720;

	};

	// Owns the renderer and render system.
	// Must be initialized after PlatformModule and ResourceModule.
	class LUM_API RenderModule {
	public:

		render::Renderer	 m_Renderer{};
		render::RenderSystem m_RenderSys{};

		void Initialize( const RenderModuleCreateInfo& info );

	};

} // namespace lum