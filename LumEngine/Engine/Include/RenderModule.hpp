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

	// Owns the renderer and render system.
	// Must be initialized after PlatformModule and ResourceModule.
	class LUM_API RenderModule {
	public:

		render::Renderer	 mRenderer{};
		render::RenderSystem mRenderSys{};

		void Initialize( PlatformModule& platform, ResourceModule& res, ev::EventBus& bus );

	};

} // namespace lum