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

		SafePtr<rhi::RenderDevice> mRenderDev = nullptr;
		SafePtr<ResourceModule> mResourceModule = nullptr;
		SafePtr<ev::EventBus> mEventBus = nullptr;
		uint32 mViewportWidth = 1280;
		uint32 mViewportHeight = 720;

	};

	// Owns the renderer and render system.
	// Must be initialized after PlatformModule and ResourceModule.
	class LUM_API RenderModule {
	public:

		render::Renderer	 mRenderer{};
		render::RenderSystem mRenderSys{};

		void Initialize( const RenderModuleCreateInfo& info );

	};

} // namespace lum